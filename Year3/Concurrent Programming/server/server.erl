-module(server).
-export([start/1]).

%% Dimensões do mundo
-define(WORLD_W, 800).% largura do mundo
-define(WORLD_H, 600).% altura

%% Física
-define(FORCA,         150.0).% força de aceleração
-define(TORQUE,        3.0).% força de rotação
-define(MASSA_INICIAL, 400.0).
-define(MASSA_MINIMA,  100.0).
-define(FRICCAO,       0.90).% multiplicador de fricção (90% da velocidade fica)
-define(TICK_MS,       50).% de quantos em quantos ms a física avança(20 vezes/s)
-define(DURACAO_JOGO,  120).

%% Objetos e partidas
-define(NUM_OBJETOS_INICIAL, 15).% comida no mapa
-define(MAX_PARTIDAS,        4).% máximo de partidas em simultâneo

%% ─────────────────────────────────────────────────────────────────
%% 1. ARRANQUE
%% ─────────────────────────────────────────────────────────────────
start(Port) ->
    spawn(fun() -> init_server(Port) end).

init_server(Port) -> %o servidor abre um socket TCP na porta indicada com as opções abaixo
    {ok, LSock} = gen_tcp:listen(Port, [binary, {packet, line}, {reuseaddr, true}, {active, true}]), %A opção {packet, line} separa as mensagens recebidas por linhas (\n).
    io:format("Servidor a escutar na porta ~p~n", [Port]),
    UsersMap     = load_accounts(),
    io:format("Contas carregadas: ~p~n", [maps:size(UsersMap)]),
    TopScoresPid = spawn(fun() -> top_scores_loop([]) end), %Guarda a tabela dos 10 melhores
    LobbyPid     = spawn(fun() -> lobby_loop([], undefined, #{}, 0, TopScoresPid) end), %Gere a fila de espera e quantas partidas estão a decorrer
    UsersPid     = spawn(fun() -> main_loop(UsersMap, LobbyPid) end), %Trata de login/registo/contas
    spawn(fun() -> acceptor(LSock, UsersPid) end), %Aceita novas ligações TCP
    wait_loop().

wait_loop() ->
    receive
        stop -> ok;
        _    -> wait_loop()
    end. %O wait_loop() no fim serve apenas para manter o processo init_server vivo, à espera de um sinal stop.

%% ─────────────────────────────────────────────────────────────────
%% 2. ACCEPTOR
%% ─────────────────────────────────────────────────────────────────
acceptor(LSock, UsersPid) ->
    {ok, Sock} = gen_tcp:accept(LSock),% fica bloqueado à espera que um cliente se ligue. Quando alguém se liga, recebe o socket desse cliente (Sock) e continua.
    io:format("Novo jogador conectado~n"),
    spawn(fun() -> acceptor(LSock, UsersPid) end),% antes de tratar o cliente, cria  um clone de si próprio para ficar à espera do próximo cliente.
    client(Sock, UsersPid).% trata do cliente que acabou de ligar.
% Resultado: o servidor consegue aceitar dezenas de clientes em paralelo, cada um tratado pelo seu próprio processo.
%% ─────────────────────────────────────────────────────────────────
%% 3. PROCESSO CLIENTE
%% ─────────────────────────────────────────────────────────────────
client(Sock, UsersPid) ->
    receive %receive bloqueia o processo à espera de uma mensagem
        {tcp, Sock, Data} ->
            String = string:trim(binary_to_list(Data)), %converte os bytes recebidos em texto
            Msg = parse_input(String, Sock), %interpreta o comando (ex: "login#gui#1234")
            UsersPid ! Msg, %envia a mensagem interpretada para o processo de utilizadores
            client(Sock, UsersPid); %chama-se a si próprio para continuar à escuta
        {tcp_closed, Sock} ->
            io:format("Cliente desconectou~n");
        {error, _Sock, _Reason} ->
            ok % Erro qualquer termina
    end.

%% ─────────────────────────────────────────────────────────────────
%% 4. PARSE INPUT
%% ─────────────────────────────────────────────────────────────────
parse_input(String, Sock) -> %pega numa string crua que veio do jogador via TCP e transforma-a numa mensagem Erlang
    case string:lexemes(String, "#") of %divide a string pelo separador #
        ["register",   User, Pass] -> {register,   User, Pass, Sock};
        ["unregister", User, Pass] -> {unregister, User, Pass, Sock};
        ["login",      User, Pass] -> {login,      User, Pass, Sock};
        ["join_lobby", User]       -> {join_lobby, User, Sock};
        ["key_press",  User, Key]  -> {key_press,  User, Key};
        ["key_release",User, Key]  -> {key_release,User, Key};
        _                          -> {comando_desconhecido, String, Sock}
    end.

%% ─────────────────────────────────────────────────────────────────
%% 5. MAIN LOOP
%% ─────────────────────────────────────────────────────────────────
main_loop(UsersMap, LobbyPid) -> %Recebe mensagens estruturadas (vindas do client via parse_input) e trata
    receive
        {register, User, Pass, Sock} ->
            case maps:is_key(User, UsersMap) of
                true -> %Verifica se o utilizador já existe
                    gen_tcp:send(Sock, <<"ERR#Utilizador ja existe\n">>),
                    main_loop(UsersMap, LobbyPid); %recursão no fim é o loop
                false -> %Se não existir, adiciona ao mapa, guarda em ficheiro, e responde OK#REGISTER.
                    NovoMapa = maps:put(User, {Pass, 0, 0}, UsersMap),
                    save_accounts(NovoMapa),
                    gen_tcp:send(Sock, <<"OK#REGISTER\n">>),
                    io:format("Registo: ~p~n", [User]),
                    main_loop(NovoMapa, LobbyPid)
            end;

        {unregister, User, Pass, Sock} -> %apaga se utilizador e passe corretas 
            case maps:find(User, UsersMap) of
                {ok, {PassGuardada, _, _}} when PassGuardada == Pass ->
                    NovoMapa = maps:remove(User, UsersMap),
                    save_accounts(NovoMapa),
                    gen_tcp:send(Sock, <<"OK#UNREGISTER\n">>),
                    io:format("Registo cancelado: ~p~n", [User]),
                    main_loop(NovoMapa, LobbyPid);
                {ok, _} ->
                    gen_tcp:send(Sock, <<"ERR#Password incorreta\n">>),
                    main_loop(UsersMap, LobbyPid);
                error ->
                    gen_tcp:send(Sock, <<"ERR#Utilizador nao encontrado\n">>),
                    main_loop(UsersMap, LobbyPid)
            end;

        {login, User, Pass, Sock} -> %Mesma lógica do unregister mas sem alterar nada, só verifica e responde.
            case maps:find(User, UsersMap) of
                {ok, {PassGuardada, _, _}} when PassGuardada == Pass ->
                    gen_tcp:send(Sock, <<"OK#LOGIN\n">>),
                    io:format("Login: ~p~n", [User]),
                    main_loop(UsersMap, LobbyPid);
                {ok, _} ->
                    gen_tcp:send(Sock, <<"ERR#Password incorreta\n">>),
                    main_loop(UsersMap, LobbyPid);
                error ->
                    gen_tcp:send(Sock, <<"ERR#Utilizador nao encontrado\n">>),
                    main_loop(UsersMap, LobbyPid)
            end;

        {join_lobby, User, Sock} -> %O main_loop não trata destas mensagens,apenas as reencaminha para o LobbyPid
            LobbyPid ! {entrar_fila, User, Sock},
            main_loop(UsersMap, LobbyPid); %recursão no fim é o loop

        {key_press, User, Key} ->
            LobbyPid ! {key_press, User, Key},
            main_loop(UsersMap, LobbyPid);

        {key_release, User, Key} ->
            LobbyPid ! {key_release, User, Key},
            main_loop(UsersMap, LobbyPid);

        {comando_desconhecido, _String, Sock} ->
            gen_tcp:send(Sock, <<"ERR#Comando desconhecido\n">>),
            main_loop(UsersMap, LobbyPid)
    end.

%% ─────────────────────────────────────────────────────────────────
%% 6. LOBBY LOOP
%% ─────────────────────────────────────────────────────────────────
lobby_loop(Queue, TimerRef, ActiveGamesMap, NumPartidas, TopScoresPid) ->
    receive %Gere a fila de espera e decide quando arrancar partidas
        {entrar_fila, User, Sock} -> %Quando alguém entra
            TopScoresPid ! {get_top, Sock}, %envia-lhe imediatamente o top scores
            NovaQueue    = Queue ++ [{User, Sock}],
            NumJogadores = length(NovaQueue), %aumenta queue
            io:format("Lobby: ~p na fila. Total: ~p, Partidas ativas: ~p~n",
                      [User, NumJogadores, NumPartidas]),
            gen_tcp:send(Sock, <<"LOBBY#OK\n">>), %e confirma LOBBY#OK

            CanStart = NumPartidas < ?MAX_PARTIDAS, % pode começar se nao passar o n de partidas max
            if
                NumJogadores == 4 andalso CanStart -> %e tivermos 4 jogadores
                    if TimerRef =/= undefined -> erlang:cancel_timer(TimerRef); true -> ok end, %Cancela o timer se existia
                    {Jogadores, Resto} = lists:split(4, NovaQueue), %Tira os 4 primeiros da fila com lists:split
                    Self = self(),
                    GamePid = spawn(fun() -> init_game(Jogadores, TopScoresPid, Self) end), %Cria um novo processo GamePid para a partida
                    NovoMapa = registar_jogadores(Jogadores, GamePid, ActiveGamesMap), %Regista esses 4 jogadores no ActiveGamesMap apontando para o GamePid
                    lobby_loop(Resto, undefined, NovoMapa, NumPartidas + 1, TopScoresPid); %Continua com o resto da fila

                NumJogadores == 3 andalso CanStart -> % se 3 players
                    NovoTimer = erlang:send_after(10000, self(), tempo_esgotado), %daqui a 10 segundos envia a mensagem tempo_esgotado ao lobby
                    lobby_loop(NovaQueue, NovoTimer, ActiveGamesMap, NumPartidas, TopScoresPid);
                    %Se entretanto chegar um 4º jogador, o timer é cancelado e arranca logo.
                true ->
                    lobby_loop(NovaQueue, TimerRef, ActiveGamesMap, NumPartidas, TopScoresPid)
            end;

        tempo_esgotado ->
            NumJogadores = length(Queue),
            CanStart = NumPartidas < ?MAX_PARTIDAS,
            if
                NumJogadores >= 3 andalso CanStart -> %Passaram 10 segundos.Se ainda houver 3+ jogadores, arranca com o que há 
                    {Jogadores, Resto} = lists:split(min(4, NumJogadores), Queue), %mesma forma de acima
                    Self = self(),
                    GamePid = spawn(fun() -> init_game(Jogadores, TopScoresPid, Self) end),
                    NovoMapa = registar_jogadores(Jogadores, GamePid, ActiveGamesMap),
                    lobby_loop(Resto, undefined, NovoMapa, NumPartidas + 1, TopScoresPid);
                true ->
                    lobby_loop(Queue, undefined, ActiveGamesMap, NumPartidas, TopScoresPid)
            end;

        {game_over} -> %decrementa o contador de partidas ativas
            io:format("Lobby: partida terminou. Partidas ativas: ~p~n", [NumPartidas - 1]),
            lobby_loop(Queue, TimerRef, ActiveGamesMap, NumPartidas - 1, TopScoresPid);

        {key_press, User, Key} -> % O lobby sabe em que GamePid está cada jogador
            case maps:find(User, ActiveGamesMap) of
                {ok, GamePid} -> GamePid ! {key_press, User, Key}; %Reencaminha a tecla para o processo certo
                error         -> ok %Se o jogador não estiver em jogo, ignora
            end,
            lobby_loop(Queue, TimerRef, ActiveGamesMap, NumPartidas, TopScoresPid);

        {key_release, User, Key} -> %igual acima
            case maps:find(User, ActiveGamesMap) of
                {ok, GamePid} -> GamePid ! {key_release, User, Key};
                error         -> ok
            end,
            lobby_loop(Queue, TimerRef, ActiveGamesMap, NumPartidas, TopScoresPid)
    end.

%% ─────────────────────────────────────────────────────────────────
%% 7. TOP SCORES
%% Só o VENCEDOR entra no top (não empates, não score 0)
%% ─────────────────────────────────────────────────────────────────
top_scores_loop(Scores) -> %Scores é uma lista de tuplos ordenada por pontuação
    receive
        {add_score, User, Pontos} ->
            Antigo = proplists:get_value(User, Scores, -1), %procura a pontuação anterior do jogador, ou -1 se for a primeira vez
            NovoScores = if
                Pontos > Antigo -> %Só atualiza se a nova pontuação for melhor que a anterior
                    lists:sort( %adiciona a nova
                        fun({_, A}, {_, B}) -> A >= B end, %ordena do maior para o menor
                        [{User, Pontos} | lists:keydelete(User, 1, Scores)] %remove a entrada antiga com lists:keydelete
                    );
                true -> Scores
            end,
            top_scores_loop(lists:sublist(NovoScores, 10)); %guarda apenas os top 10

        {get_top, Sock} -> %Quando um jogador entra no lobby, o lobby pede o top scores para este processo
            Msg = formatar_top(Scores),
            gen_tcp:send(Sock, list_to_binary("TOP#" ++ Msg ++ "\n")), %responde diretamente ao socket do jogador
            top_scores_loop(Scores)
    end.

formatar_top([]) -> "vazio"; %Transforma a lista numa string para enviar via TCP
formatar_top(Scores) ->
    Entradas = lists:map(fun({U, P}) ->
        U ++ ":" ++ integer_to_list(P)
    end, lists:sublist(Scores, 10)),
    string:join(Entradas, "|").

%% ─────────────────────────────────────────────────────────────────
%% 8. OBJETOS: {Id, X, Y, Massa, Tipo}
%%
%% ENUNCIADO: "objectos comestíveis de diferentes tamanhos,
%%             menores E maiores do que os jogadores"
%% Garantimos isso na criação inicial:
%%   - metade dos comestíveis com massa < MASSA_INICIAL (menores)
%%   - pelo menos 3 comestíveis com massa > MASSA_INICIAL (maiores)
%% ─────────────────────────────────────────────────────────────────
novo_objeto_pequeno(Id) -> %Cada objeto é um tuplo com 5 campos: {Id, X, Y, Massa, Tipo}
    X    = float(rand:uniform(?WORLD_W - 40) + 20),
    Y    = float(rand:uniform(?WORLD_H - 40) + 20),
    Massa = float(rand:uniform(round(?MASSA_INICIAL) - 20) + 20), %% 20..399
    {Id, X, Y, Massa, comestivel}.

novo_objeto_grande(Id) ->
    X    = float(rand:uniform(?WORLD_W - 40) + 20), %A massa inicial do jogador é 400
    Y    = float(rand:uniform(?WORLD_H - 40) + 20), %por isso os pequenos são sempre menores e os grandes são sempre maiores
    Massa = ?MASSA_INICIAL + float(rand:uniform(400) + 50), %% 450..850
    {Id, X, Y, Massa, comestivel}.

novo_objeto_venenoso(Id) ->
    X    = float(rand:uniform(?WORLD_W - 40) + 20),
    Y    = float(rand:uniform(?WORLD_H - 40) + 20),
    Massa = float(rand:uniform(300) + 50), %% 50..350
    {Id, X, Y, Massa, venenoso}.

novo_objeto_aleatorio(Id) -> %Quando um jogador captura um objeto, este é substituído por um aleatório.
    X    = float(rand:uniform(?WORLD_W - 40) + 20),
    Y    = float(rand:uniform(?WORLD_H - 40) + 20),
    Massa = float(rand:uniform(750) + 50),
    Tipo  = case rand:uniform(3) of %Há 1 em 3 hipóteses de ser venenoso.
                1 -> venenoso;
                _ -> comestivel
            end,
    {Id, X, Y, Massa, Tipo}.

%% Cria o conjunto inicial garantindo menores E maiores que os jogadores
criar_objetos_iniciais(N) -> %Com N = 15 (o valor da constante ?NUM_OBJETOS_INICIAL), o mapa começa sempre com:
    %% 3 grandes, 3 venenosos, resto aleatórios (todos menores ou mistos)
    Grandes  = [novo_objeto_grande(I)   || I <- lists:seq(1, 3)], %é uma list comprehension equivale a um map/for que gera uma lista.
    Venenosos= [novo_objeto_venenoso(I) || I <- lists:seq(4, 6)],
    Pequenos = [novo_objeto_pequeno(I)  || I <- lists:seq(7, 7 + max(0, N-7))],
    Grandes ++ Venenosos ++ Pequenos.

garantir_pequeno(Objetos, GameState, ProxId) -> %Esta função é chamada a cada tick do jogo
    MenorMassaJogador = maps:fold(fun(_, {_, _, M, _, _, _, _, _, _}, Acc) ->
        if M < Acc -> M; true -> Acc end
    end, 999999.0, GameState),
    TemPequeno = lists:any(fun({_, _, _, M, T}) ->
        T == comestivel andalso M < MenorMassaJogador %Verifica se existe pelo menos um objeto comestível mais pequeno que o jogador mais leve
    end, Objetos),
    if
        TemPequeno -> {Objetos, ProxId};
        true ->
            X = float(rand:uniform(?WORLD_W - 40) + 20),
            Y = float(rand:uniform(?WORLD_H - 40) + 20),
            MassaPequena = max(20.0, MenorMassaJogador * 0.4),
            {[{ProxId, X, Y, MassaPequena, comestivel} | Objetos], ProxId + 1}
    end.

%% ─────────────────────────────────────────────────────────────────
%% 9. INIT GAME
%% Estado jogador: {X, Y, Massa, Angulo, VX, VY, VAng, Score, Sock}
%% ─────────────────────────────────────────────────────────────────
init_game(Jogadores, TopScoresPid, LobbyPid) -> 
    io:format("GameRoom (~p): a iniciar com ~p jogadores~n", [self(), length(Jogadores)]),
    rand:seed(exsss, {erlang:system_time(), erlang:phash2(self()), erlang:phash2(make_ref())}), %Seed aleatória
    EstadoInicial   = setup_jogadores(Jogadores, #{}, ?WORLD_W div (length(Jogadores) + 1)), %Posições iniciais dos jogadores
    TeclasIniciais  = setup_teclas(Jogadores, #{}),
    ObjetosIniciais = criar_objetos_iniciais(?NUM_OBJETOS_INICIAL),
    avisar_game_start(maps:to_list(EstadoInicial)),
    erlang:send_after(?TICK_MS, self(), tick), %Agenda o primeiro tick daqui a 50ms
    game_loop(EstadoInicial, TeclasIniciais, ObjetosIniciais, %passa o controlo para o game_loop
              ?DURACAO_JOGO * (1000 div ?TICK_MS),
              ?NUM_OBJETOS_INICIAL + 1, TopScoresPid, LobbyPid).

setup_jogadores([], Mapa, _) -> Mapa; %O estado inicial de cada jogador é o tuplo {X, Y, Massa, Angulo, VX, VY, VAng, Score, Sock}
setup_jogadores([{User, Sock} | Resto], Mapa, PosX) ->
    Estado = {float(PosX), float(?WORLD_H div 2), ?MASSA_INICIAL, 0.0, 0.0, 0.0, 0.0, 0, Sock},
    setup_jogadores(Resto, maps:put(User, Estado, Mapa), PosX + ?WORLD_W div 5).

setup_teclas([], Mapa) -> Mapa; %Cada jogador começa sem nenhuma tecla pressionada
setup_teclas([{User, _Sock} | Resto], Mapa) ->
    setup_teclas(Resto, maps:put(User, #{left => false, right => false, forward => false}, Mapa)).

avisar_game_start([]) -> ok; %Envia GAME_START#gui\n a cada jogador 
avisar_game_start([{User, {_,_,_,_,_,_,_,_,Sock}} | Resto]) -> % o cliente Java usa isto para saber o seu próprio nome
    gen_tcp:send(Sock, list_to_binary("GAME_START#" ++ User ++ "\n")),% e mudar do ecrã de lobby para o ecrã de jogo
    avisar_game_start(Resto).
    %O init_game Corre uma única vez, prepara o estado inicial completo, avisa todos os jogadores, e entrega tudo ao game_loop
%% ─────────────────────────────────────────────────────────────────
%% 10. GAME LOOP
%% ─────────────────────────────────────────────────────────────────
game_loop(GameState, Teclas, Objetos, TicksRestantes, ProxId, TopScoresPid, LobbyPid) ->
    receive
        tick ->
            if %A cada tick, o jogo executa 6 passos em sequência
                TicksRestantes =< 0 ->
                    terminar_jogo(GameState, TopScoresPid, LobbyPid);
                true ->
                    State1 = aplicar_fisica(GameState, Teclas), %física aplicada
                    {State2, Objetos2, ProxId2} = processar_colisoes(State1, Objetos, ProxId), %colisões com objetos resolvidas
                    {State3, ProxId3}           = processar_capturas_jogadores(State2, ProxId2), %capturas entre jogadores resolvidas
                    State4 = aplicar_repulsao_global(State3), %repulsão entre jogadore,garantir objeto pequeno,broadcast para todos os clientes
                    {Objetos3, ProxId4}         = garantir_pequeno(Objetos2, State4, ProxId3),%e agendar próximo tick
                    TempoRestante = (TicksRestantes * ?TICK_MS) div 1000,
                    broadcast_state(maps:to_list(State4), maps:to_list(State4), Objetos3, TempoRestante),
                    erlang:send_after(?TICK_MS, self(), tick),
                    game_loop(State4, Teclas, Objetos3, TicksRestantes - 1, ProxId4, TopScoresPid, LobbyPid)
            end;

        {key_press, User, Key} -> %Não altera o GameState apenas atualiza o mapa de Teclas
            game_loop(GameState, atualizar_tecla(User, Key, true, Teclas),
                      Objetos, TicksRestantes, ProxId, TopScoresPid, LobbyPid);

        {key_release, User, Key} ->%Mesma lógica, marca a tecla como false
            game_loop(GameState, atualizar_tecla(User, Key, false, Teclas),
                      Objetos, TicksRestantes, ProxId, TopScoresPid, LobbyPid);

        _ ->
            game_loop(GameState, Teclas, Objetos, TicksRestantes, ProxId, TopScoresPid, LobbyPid)
    end.

%% ─────────────────────────────────────────────────────────────────
%% 11. FÍSICA
%%
%% Ao bater numa parede vertical (esq/dir) VX fica 0 mas
%% mantemos VY; ao bater numa parede horizontal (cima/baixo)
%% VY fica 0 mas mantemos VX.
%% ─────────────────────────────────────────────────────────────────
aplicar_fisica(GameState, Teclas) ->
    maps:map(fun(User, Estado) -> %percorre todos os jogadores e aplica aplicar_fisica_jogador a cada um individualmente
        TeclasUser = maps:get(User, Teclas, #{left=>false, right=>false, forward=>false}),
        aplicar_fisica_jogador(Estado, TeclasUser)
    end, GameState).

aplicar_fisica_jogador({X, Y, Massa, Angulo, VX, VY, VAng, Score, Sock}, Teclas) ->
    AcelAngular = ?TORQUE / Massa, %A aceleração é inversamente proporcional à massa
    AcelLinear  = ?FORCA  / Massa,

    %Rotação
    DeltaAng = case {maps:get(left, Teclas, false), maps:get(right, Teclas, false)} of
        {true, false} -> -AcelAngular; %Seta esquerda → ângulo decresce
        {false, true} ->  AcelAngular; %Seta direita → ângulo cresce
        _             ->  0.0
    end,
    NovoVAng   = (VAng + DeltaAng) * ?FRICCAO, %é a velocidade angular, acumula-se e é travada pela fricção (* 0.90)
    NovoAngulo = Angulo + NovoVAng,

    {DVX, DVY} = case maps:get(forward, Teclas, false) of %Quando a seta cima está pressionada a força é aplicada na direção em que o jogador está a apontar
        true  -> {AcelLinear * math:cos(NovoAngulo), AcelLinear * math:sin(NovoAngulo)};
        false -> {0.0, 0.0}
    end,
    NovoVX = (VX + DVX) * ?FRICCAO,
    NovoVY = (VY + DVY) * ?FRICCAO,

    %Colisão com paredes
    Raio = raio(Massa), % o raio do jogador o centro não pode ir além de Raio pixels da borda. Ao bater numa parede:
    MinX = Raio,
    MaxX = float(?WORLD_W) - Raio,
    MinY = Raio,
    MaxY = float(?WORLD_H) - Raio,

    NovaPosX = X + NovoVX,
    NovaPosY = Y + NovoVY,

    %% CORRIGIDO: parede vertical zera só VX, mantém VY (tangencial)
    {FinalX, FinalVX} = if
        NovaPosX < MinX -> {MinX, 0.0};
        NovaPosX > MaxX -> {MaxX, 0.0};
        true            -> {NovaPosX, NovoVX}
    end,
    %% CORRIGIDO: parede horizontal zera só VY, mantém VX (tangencial)
    {FinalY, FinalVY} = if
        NovaPosY < MinY -> {MinY, 0.0};
        NovaPosY > MaxY -> {MaxY, 0.0};
        true            -> {NovaPosY, NovoVY}
    end,

    {FinalX, FinalY, Massa, NovoAngulo, FinalVX, FinalVY, NovoVAng, Score, Sock}.

raio(Massa)             -> math:sqrt(Massa / math:pi()).
distancia(X1,Y1,X2,Y2) -> math:sqrt((X2-X1)*(X2-X1) + (Y2-Y1)*(Y2-Y1)).
posicao_aleatoria()     -> {float(rand:uniform(?WORLD_W - 80) + 40),
                             float(rand:uniform(?WORLD_H - 80) + 40)}.

%% ─────────────────────────────────────────────────────────────────
%% 12. COLISÕES JOGADOR <-> OBJETOS
%%
%%   - venenoso: QUALQUER sobreposição (dist < raioJ + raioO)
%%   - comestivel: sobreposição COMPLETA (dist + raioO <= raioJ)
%%     E o jogador tem de ser maior (raioJ > raioO)
%% ─────────────────────────────────────────────────────────────────
processar_colisoes(GameState, Objetos, ProxId) ->
    maps:fold(fun(User, {X,Y,Massa,Ang,VX,VY,VAng,Score,Sock}, {StateAcc, ObjsAcc, IdAcc}) -> %percorre cada jogador
        RaioJ = raio(Massa),

        %% Separa venenosos que sobrepõem (qualquer sobreposição)
        %%    e comestíveis que foram completamente cobertos (sobreposição total)
        {Capturados, Restantes} = lists:partition(fun({_,OX,OY,OM,Tipo}) -> %Para cada um, usa lists:partition para dividir todos os objetos em dois grupos
            RaioO = raio(OM), %os que foram capturados e os que ficam no mapa
            Dist  = distancia(X, Y, OX, OY),
            case Tipo of
                venenoso   -> Dist < (RaioJ + RaioO);          %% qualquer sobreposição
                comestivel -> RaioJ > RaioO andalso            %% jogador maior
                              (Dist + RaioO) =< RaioJ          %% sobreposição completa
            end
        end, ObjsAcc),

        {NovaMassa, NovosObjs, NovoId} = lists:foldl(
            fun({_,_,_,OM,Tipo}, {MAcc, OAcc, IDAcc}) ->
                case Tipo of
                    comestivel ->
                        %% captura toda a massa do objeto comestível
                        NovoObj = novo_objeto_aleatorio(IDAcc), %substituido por um novo objeto random
                        {MAcc + OM, [NovoObj | OAcc], IDAcc + 1};
                    venenoso ->
                        %% penaliza: subtrai massa do objeto, nunca abaixo do mínimo
                        {max(?MASSA_MINIMA, MAcc - OM), OAcc, IDAcc}
                end
            end, {Massa, [], IdAcc}, Capturados),

        NovoEstado = {X, Y, NovaMassa, Ang, VX, VY, VAng, Score, Sock}, %Apenas a massa muda posição, velocidade, ângulo e score mantêm-se
        {maps:put(User, NovoEstado, StateAcc), Restantes ++ NovosObjs, max(NovoId, IdAcc + 1)}

    end, {GameState, Objetos, ProxId}, GameState).

%% ─────────────────────────────────────────────────────────────────
%% 13. CAPTURAS JOGADOR <-> JOGADOR
%% ─────────────────────────────────────────────────────────────────
processar_capturas_jogadores(GameState, ProxId) ->
    Jogadores = maps:to_list(GameState), %Converte o mapa de jogadores numa lista
    processar_pares(Jogadores, GameState, ProxId). %e passa para processar_pares, que vai comparar todos os pares possíveis

%Comparação de todos os pares
processar_pares([], GS, Id) -> {GS, Id};
processar_pares([{UA, _} | Resto], GS, Id) ->
    {GS2, Id2} = processar_contra(UA, Resto, GS, Id),
    processar_pares(Resto, GS2, Id2).

processar_contra(_UA, [], GS, Id) -> {GS, Id}; %Primeiro verifica se ambos ainda existem no estado
processar_contra(UA, [{UB, _} | Resto], GS, Id) -> %Depois calcula os raios e a distância entre centros
    case {maps:get(UA, GS, undefined), maps:get(UB, GS, undefined)} of
        {undefined, _} -> processar_contra(UA, Resto, GS, Id);
        {_, undefined} -> processar_contra(UA, Resto, GS, Id);
        {{XA,YA,MA,AngA,VXA,VYA,VAA,ScA,SockA},
         {XB,YB,MB,AngB,_VXB,_VYB,_VAB,ScB,SockB}} ->
            RA = raio(MA), RB = raio(MB),
            D  = distancia(XA, YA, XB, YB),
            %Condições de captura
            GS2 = if
                %% A cobre completamente B (A maior, dist + RB <= RA)
                MA > MB andalso (D + RB) =< RA ->
                    T = MB / 4.0,
                    {RX, RY} = posicao_aleatoria(),
                    GS#{UA => {XA,YA,MA+T,AngA,VXA,VYA,VAA,ScA+1,SockA},
                        UB => {RX,RY,max(?MASSA_MINIMA,MB-T),AngB,0.0,0.0,0.0,ScB,SockB}};
                %% B cobre completamente A (B maior, dist + RA <= RB)
                MB > MA andalso (D + RA) =< RB ->
                    T = MA / 4.0,
                    {RX, RY} = posicao_aleatoria(),
                    GS#{UA => {RX,RY,max(?MASSA_MINIMA,MA-T),AngA,0.0,0.0,0.0,ScA,SockA},
                        UB => {XB,YB,MB+T,AngB,VXA,VYA,VAA,ScB+1,SockB}};
                true -> GS
            end,
            processar_contra(UA, Resto, GS2, Id)
    end.

%% ─────────────────────────────────────────────────────────────────
%% 14. BROADCAST
%% 
%% Enviar o estado completo do jogo a todos os jogadores a cada tick
%% ─────────────────────────────────────────────────────────────────
broadcast_state([], _, _, _) -> ok; %Percorre a lista de jogadores recursivamente e envia a mesma mensagem a cada um via TCP
broadcast_state([{_,{_,_,_,_,_,_,_,_,Sock}} | Resto], Todos, Objs, Tempo) ->
    gen_tcp:send(Sock, list_to_binary(formatar_state(Todos, Objs, Tempo))),
    broadcast_state(Resto, Todos, Objs, Tempo).

formatar_state(Jogadores, Objetos, Tempo) -> %Constrói uma string com o formato: STATE#tempo#jogadores#objetos\n
    PJ = string:join([lists:flatten(io_lib:format("~s:~.1f:~.1f:~.1f:~.4f:~p",% nome : X : Y : massa : ângulo : score
            [U,X,Y,M,A,S])) || {U,{X,Y,M,A,_,_,_,S,_}} <- Jogadores], "|"),
    PO = case Objetos of
        [] -> "none";
        _  -> string:join([lists:flatten(io_lib:format("~p:~.1f:~.1f:~.1f:~s",% id : X : Y : massa : tipo(c/v)
                [Id,X,Y,M, case T of comestivel->"c"; venenoso->"v" end]))
              || {Id,X,Y,M,T} <- Objetos], "|")
    end,
    io_lib:format("STATE#~p#~s#~s~n", [Tempo, PJ, PO]).
% exemplo : STATE#87#gui:320.0:300.0:450.0:1.5708:2|ana:160.0:300.0:380.0:0.0000:0#1:200.0:150.0:80.0:c|2:400.0:500.0:200.0:v\n
%% ─────────────────────────────────────────────────────────────────
%% 15. TECLAS
%%  Esta secção gere o estado das teclas pressionadas por cada jogador
%% ─────────────────────────────────────────────────────────────────
atualizar_tecla(User, Key, Valor, Teclas) ->
    TU = maps:get(User, Teclas, #{left=>false, right=>false, forward=>false}),%busca o mapa de teclas do jogador, ou cria um vazio se não existir
    maps:put(User, maps:put(key_atom(Key), Valor, TU), Teclas).% atualiza a tecla específica dentro do mapa do jogador (true ou false) e devolve o mapa global atualizado

%Conversão de string para átomo
key_atom("LEFT")    -> left; %O cliente Java envia strings como "LEFT", "RIGHT", "FORWARD" via TCP
key_atom("RIGHT")   -> right;
key_atom("FORWARD") -> forward;
key_atom(_)         -> desconhecido.

%% ─────────────────────────────────────────────────────────────────
%% 16. FIM DE JOGO
%% ─────────────────────────────────────────────────────────────────
terminar_jogo(GameState, TopScoresPid, LobbyPid) ->
    io:format("GameRoom (~p): Fim de jogo!~n", [self()]),
    Jogadores = maps:to_list(GameState),
    {Vencedor, MaiorScore} = encontrar_vencedor(Jogadores), %Primeiro encontra o vencedor e o score mais alto

    %% Conta quantos jogadores têm o score máximo
    NumComMaiorScore = length([U || {U,{_,_,_,_,_,_,_,S,_}} <- Jogadores, S == MaiorScore]),
    Empate = NumComMaiorScore > 1,

    Msg = if
        Empate -> % se for mais de um, é empate
            "GAME_OVER#Empate! Partida ignorada no top de pontuacoes.";
        MaiorScore =:= 0 ->
            "GAME_OVER#Nenhum jogador fez capturas. Partida ignorada no top.";
        true ->
            %% Só aqui adicionamos ao top (vencedor único, com score > 0)
            TopScoresPid ! {add_score, Vencedor, MaiorScore},
            "GAME_OVER#Vencedor: " ++ Vencedor ++
            " com " ++ integer_to_list(MaiorScore) ++ " capturas!"
    end,

    lists:foreach(fun({_,{_,_,_,_,_,_,_,_,Sock}}) -> %Envia a mesma mensagem GAME_OVER#... a todos os sockets
        gen_tcp:send(Sock, list_to_binary(Msg ++ "\n"))
    end, Jogadores),
    LobbyPid ! {game_over}.

encontrar_vencedor(Jogadores) ->
    lists:foldl(fun({U,{_,_,_,_,_,_,_,S,_}}, {BU,BS}) -> %percorre todos os jogadores mantendo o melhor encontrado até ao momento 
        if S > BS -> {U,S}; true -> {BU,BS} end
    end, {"", -1}, Jogadores). %Começa com {"", -1} para que qualquer score real seja melhor

%% ─────────────────────────────────────────────────────────────────
%% 17. REPULSÃO entre jogadores (evita sobreposição visual)
%% ─────────────────────────────────────────────────────────────────
aplicar_repulsao_global(GameState) ->
    ListaJogadores = maps:to_list(GameState),
    todas_repulsoes(ListaJogadores, GameState). %Converte o estado em lista e passa para todas_repulsoes
    % que compara todos os pares — exatamente como as capturas da secção 13

%Percorrer todos os pares:
todas_repulsoes([], GS) -> GS;
todas_repulsoes([{UA, _} | Resto], GS) ->
    GS2 = repulsao_contra_resto(UA, Resto, GS),
    todas_repulsoes(Resto, GS2).

repulsao_contra_resto(_UA, [], GS) -> GS;
repulsao_contra_resto(UA, [{UB, _} | Resto], GS) ->
    case {maps:get(UA, GS), maps:get(UB, GS)} of
        {{XA,YA,MA,AngA,VXA,VYA,VAA,ScA,SockA},
         {XB,YB,MB,AngB,VXB,VYB,VAB,ScB,SockB}} ->
            RA = raio(MA), RB = raio(MB),
            Dist = distancia(XA, YA, XB, YB),
            MinDist = RA + RB,
            DiferencaMassaSuficiente = (MA > MB * 1.2) orelse (MB > MA * 1.2), %Calcula os raios, a distância, e verifica se a diferença de massa é suficiente para captura.
            if (Dist < MinDist) andalso (Dist > 0.001) andalso (not DiferencaMassaSuficiente) ->
                %Cálculo da força de repulsão
                Overlap = MinDist - Dist,
                DX = (XA - XB) / Dist,
                DY = (YA - YB) / Dist,
                Forca = Overlap * 0.5,
                GS2 = GS#{
                    UA => {XA + DX*Forca, YA + DY*Forca, MA, AngA, VXA, VYA, VAA, ScA, SockA},
                    UB => {XB - DX*Forca, YB - DY*Forca, MB, AngB, VXB, VYB, VAB, ScB, SockB}
                },
                repulsao_contra_resto(UA, Resto, GS2);
            true ->
                repulsao_contra_resto(UA, Resto, GS)
            end;
        _ -> GS
    end.

%% ─────────────────────────────────────────────────────────────────
%% 18. AUXILIARES
%% ─────────────────────────────────────────────────────────────────
registar_jogadores([], _, Mapa) -> Mapa;
registar_jogadores([{User, _} | Resto], GamePid, Mapa) ->
    registar_jogadores(Resto, GamePid, maps:put(User, GamePid, Mapa)).

%% ─────────────────────────────────────────────────────────────────
%% 19. PERSISTÊNCIA DE CONTAS (accounts.txt)
%% ─────────────────────────────────────────────────────────────────
load_accounts() ->
    case file:read_file("accounts.txt") of %tenta ler o ficheiro
        {ok, Binary} ->
            Lines = string:tokens(binary_to_list(Binary), "\n"), %divide o conteúdo em linhas
            lists:foldl(fun(Line, Acc) -> %percorre cada linha, divide pelo ,, e adiciona ao mapa
                case string:tokens(Line, ",") of
                    [User, Pass] -> maps:put(User, {Pass, 0, 0}, Acc);
                    _ -> Acc %Linhas mal formatadas são ignoradas silenciosamente com _ -> Acc
                end
            end, #{}, Lines);
        {error, _} ->
            #{}
    end.

save_accounts(UsersMap) ->
    Lines = maps:fold(fun(User, {Pass, _, _}, Acc) -> %maps:fold percorre o mapa e constrói uma lista de strings ["gui,1234\n", "ana,pass\n", ...]
        [User ++ "," ++ Pass ++ "\n" | Acc]
    end, [], UsersMap),
    file:write_file("accounts.txt", list_to_binary(Lines)). %escreve tudo de uma vez, substituindo o ficheiro anterior