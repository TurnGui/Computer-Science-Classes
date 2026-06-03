-module(server).
-export([start/1]).

%% Dimensões do mundo
-define(WORLD_W, 800).
-define(WORLD_H, 600).

%% Física
-define(FORCA,        150.0).
-define(TORQUE,       3.0).
-define(MASSA_INICIAL, 400.0).
-define(MASSA_MINIMA,  100.0).
-define(FRICCAO,       0.90).
-define(TICK_MS,       16).
-define(DURACAO_JOGO,  120).

%% Objetos e partidas
-define(NUM_OBJETOS_INICIAL, 15).
-define(MAX_PARTIDAS,        4).

%% ─────────────────────────────────────────────────────────────────
%% 1. ARRANQUE
%% ─────────────────────────────────────────────────────────────────
start(Port) ->
    spawn(fun() -> init_server(Port) end).

init_server(Port) ->
    {ok, LSock} = gen_tcp:listen(Port, [binary, {packet, line}, {reuseaddr, true}, {active, true}]),
    io:format("Servidor a escutar na porta ~p~n", [Port]),
    TopScoresPid = spawn(fun() -> top_scores_loop([]) end),
    LobbyPid     = spawn(fun() -> lobby_loop([], undefined, #{}, 0, TopScoresPid) end),
    UsersPid     = spawn(fun() -> main_loop(#{}, LobbyPid) end),
    spawn(fun() -> acceptor(LSock, UsersPid) end),
    wait_loop().

wait_loop() ->
    receive
        stop -> ok;
        _    -> wait_loop()
    end.

%% ─────────────────────────────────────────────────────────────────
%% 2. ACCEPTOR
%% ─────────────────────────────────────────────────────────────────
acceptor(LSock, UsersPid) ->
    {ok, Sock} = gen_tcp:accept(LSock),
    io:format("Novo jogador conectado. Socket: ~p~n", [Sock]),
    spawn(fun() -> acceptor(LSock, UsersPid) end),
    client(Sock, UsersPid).

%% ─────────────────────────────────────────────────────────────────
%% 3. PROCESSO CLIENTE
%% ─────────────────────────────────────────────────────────────────
client(Sock, UsersPid) ->
    receive
        {tcp, Sock, Data} ->
            String = string:trim(binary_to_list(Data)),
            io:format("Client (~p): '~s'~n", [self(), String]),
            Msg = parse_input(String, Sock),
            UsersPid ! Msg,
            client(Sock, UsersPid);
        {tcp_closed, Sock} ->
            io:format("Client (~p): desconectou.~n", [self()]),
            ok;
        {error, _Sock, Reason} ->
            io:format("Client (~p): erro ~p~n", [self(), Reason]),
            ok
    end.

%% ─────────────────────────────────────────────────────────────────
%% 4. PARSE INPUT
%% register#user#pass
%% unregister#user#pass
%% login#user#pass
%% join_lobby#user
%% key_press#user#LEFT|RIGHT|FORWARD
%% key_release#user#LEFT|RIGHT|FORWARD
%% ─────────────────────────────────────────────────────────────────
parse_input(String, Sock) ->
    case string:lexemes(String, "#") of
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
main_loop(UsersMap, LobbyPid) ->
    receive
        {register, User, Pass, Sock} ->
            case maps:is_key(User, UsersMap) of
                true ->
                    gen_tcp:send(Sock, <<"ERR#Utilizador ja existe\n">>),
                    main_loop(UsersMap, LobbyPid);
                false ->
                    NovoMapa = maps:put(User, {Pass, 0, 0}, UsersMap),
                    gen_tcp:send(Sock, <<"OK#REGISTER\n">>),
                    io:format("Registo: ~p~n", [User]),
                    main_loop(NovoMapa, LobbyPid)
            end;

        {unregister, User, Pass, Sock} ->
            case maps:find(User, UsersMap) of
                {ok, {PassGuardada, _, _}} when PassGuardada == Pass ->
                    NovoMapa = maps:remove(User, UsersMap),
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

        {login, User, Pass, Sock} ->
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

        {join_lobby, User, Sock} ->
            LobbyPid ! {entrar_fila, User, Sock},
            main_loop(UsersMap, LobbyPid);

        {key_press, User, Key} ->
            LobbyPid ! {key_press, User, Key},
            main_loop(UsersMap, LobbyPid);

        {key_release, User, Key} ->
            LobbyPid ! {key_release, User, Key},
            main_loop(UsersMap, LobbyPid);

        {comando_desconhecido, String, Sock} ->
            io:format("Comando desconhecido: ~p~n", [String]),
            gen_tcp:send(Sock, <<"ERR#Comando desconhecido\n">>),
            main_loop(UsersMap, LobbyPid)
    end.

%% ─────────────────────────────────────────────────────────────────
%% 6. LOBBY LOOP
%% ─────────────────────────────────────────────────────────────────
lobby_loop(Queue, TimerRef, ActiveGamesMap, NumPartidas, TopScoresPid) ->
    receive
        {entrar_fila, User, Sock} ->
            TopScoresPid ! {get_top, Sock},
            NovaQueue    = Queue ++ [{User, Sock}],
            NumJogadores = length(NovaQueue),
            io:format("Lobby: ~p na fila. Total: ~p, Partidas ativas: ~p~n",
                      [User, NumJogadores, NumPartidas]),
            gen_tcp:send(Sock, <<"LOBBY#OK\n">>),

            CanStart = NumPartidas < ?MAX_PARTIDAS,
            if
                NumJogadores == 4 andalso CanStart ->
                    if TimerRef =/= undefined -> erlang:cancel_timer(TimerRef); true -> ok end,
                    {Jogadores, Resto} = lists:split(4, NovaQueue),
                    Self = self(),
                    GamePid = spawn(fun() -> init_game(Jogadores, TopScoresPid, Self) end),
                    NovoMapa = registar_jogadores(Jogadores, GamePid, ActiveGamesMap),
                    lobby_loop(Resto, undefined, NovoMapa, NumPartidas + 1, TopScoresPid);

                NumJogadores == 3 andalso CanStart ->
                    NovoTimer = erlang:send_after(10000, self(), tempo_esgotado),
                    lobby_loop(NovaQueue, NovoTimer, ActiveGamesMap, NumPartidas, TopScoresPid);

                true ->
                    lobby_loop(NovaQueue, TimerRef, ActiveGamesMap, NumPartidas, TopScoresPid)
            end;

        tempo_esgotado ->
            NumJogadores = length(Queue),
            CanStart = NumPartidas < ?MAX_PARTIDAS,
            if
                NumJogadores >= 3 andalso CanStart ->
                    {Jogadores, Resto} = lists:split(3, Queue),
                    Self = self(),
                    GamePid = spawn(fun() -> init_game(Jogadores, TopScoresPid, Self) end),
                    NovoMapa = registar_jogadores(Jogadores, GamePid, ActiveGamesMap),
                    lobby_loop(Resto, undefined, NovoMapa, NumPartidas + 1, TopScoresPid);
                true ->
                    lobby_loop(Queue, undefined, ActiveGamesMap, NumPartidas, TopScoresPid)
            end;

        {game_over} ->
            io:format("Lobby: partida terminou. Partidas ativas: ~p~n", [NumPartidas - 1]),
            lobby_loop(Queue, TimerRef, ActiveGamesMap, NumPartidas - 1, TopScoresPid);

        {key_press, User, Key} ->
            case maps:find(User, ActiveGamesMap) of
                {ok, GamePid} -> GamePid ! {key_press, User, Key};
                error         -> ok
            end,
            lobby_loop(Queue, TimerRef, ActiveGamesMap, NumPartidas, TopScoresPid);

        {key_release, User, Key} ->
            case maps:find(User, ActiveGamesMap) of
                {ok, GamePid} -> GamePid ! {key_release, User, Key};
                error         -> ok
            end,
            lobby_loop(Queue, TimerRef, ActiveGamesMap, NumPartidas, TopScoresPid)
    end.

%% ─────────────────────────────────────────────────────────────────
%% 7. TOP SCORES
%% ─────────────────────────────────────────────────────────────────
top_scores_loop(Scores) ->
    receive
        {add_score, User, Pontos} ->
            NovoScores = lists:sort(
                fun({_, A}, {_, B}) -> A >= B end,
                [{User, Pontos} | lists:keydelete(User, 1, Scores)]
            ),
            top_scores_loop(NovoScores);

        {get_top, Sock} ->
            Msg = formatar_top(Scores),
            gen_tcp:send(Sock, list_to_binary("TOP#" ++ Msg ++ "\n")),
            top_scores_loop(Scores)
    end.

formatar_top([]) -> "vazio";
formatar_top(Scores) ->
    Entradas = lists:map(fun({U, P}) ->
        U ++ ":" ++ integer_to_list(P)
    end, lists:sublist(Scores, 10)),
    string:join(Entradas, "|").

%% ─────────────────────────────────────────────────────────────────
%% 8. OBJETOS: {Id, X, Y, Massa, Tipo}  Tipo = comestivel | venenoso
%% ─────────────────────────────────────────────────────────────────
novo_objeto(IdCounter) ->
    X    = float(rand:uniform(?WORLD_W - 40) + 20),
    Y    = float(rand:uniform(?WORLD_H - 40) + 20),
    Massa = float(rand:uniform(550) + 50),
    Tipo  = case rand:uniform(3) of
                1 -> venenoso;
                _ -> comestivel
            end,
    {IdCounter, X, Y, Massa, Tipo}.

criar_objetos_iniciais(N) ->
    criar_objetos_iniciais(N, [], 1).
criar_objetos_iniciais(0, Acc, _) -> Acc;
criar_objetos_iniciais(N, Acc, Id) ->
    criar_objetos_iniciais(N - 1, [novo_objeto(Id) | Acc], Id + 1).

garantir_pequeno(Objetos, GameState, ProxId) ->
    MenorMassaJogador = maps:fold(fun(_, {_, _, M, _, _, _, _, _, _}, Acc) ->
        if M < Acc -> M; true -> Acc end
    end, 999999.0, GameState),
    TemPequeno = lists:any(fun({_, _, _, M, T}) ->
        T == comestivel andalso M < MenorMassaJogador
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
%% Estado: {X, Y, Massa, Angulo, VX, VY, VAng, Score, Sock}
%% ─────────────────────────────────────────────────────────────────
init_game(Jogadores, TopScoresPid, LobbyPid) ->
    io:format("GameRoom (~p): a iniciar com ~p jogadores~n", [self(), length(Jogadores)]),
    rand:seed(exsss, {erlang:system_time(), erlang:phash2(self()), erlang:phash2(make_ref())}),
    EstadoInicial   = setup_jogadores(Jogadores, #{}, ?WORLD_W div (length(Jogadores) + 1)),
    TeclasIniciais  = setup_teclas(Jogadores, #{}),
    ObjetosIniciais = criar_objetos_iniciais(?NUM_OBJETOS_INICIAL),
    avisar_game_start(maps:to_list(EstadoInicial)),
    erlang:send_after(?TICK_MS, self(), tick),
    game_loop(EstadoInicial, TeclasIniciais, ObjetosIniciais,
              ?DURACAO_JOGO * (1000 div ?TICK_MS),
              ?NUM_OBJETOS_INICIAL + 1, TopScoresPid, LobbyPid).

setup_jogadores([], Mapa, _) -> Mapa;
setup_jogadores([{User, Sock} | Resto], Mapa, PosX) ->
    Estado = {float(PosX), float(?WORLD_H div 2), ?MASSA_INICIAL, 0.0, 0.0, 0.0, 0.0, 0, Sock},
    setup_jogadores(Resto, maps:put(User, Estado, Mapa), PosX + ?WORLD_W div 5).

setup_teclas([], Mapa) -> Mapa;
setup_teclas([{User, _Sock} | Resto], Mapa) ->
    setup_teclas(Resto, maps:put(User, #{left => false, right => false, forward => false}, Mapa)).

avisar_game_start([]) -> ok;
avisar_game_start([{User, {_,_,_,_,_,_,_,_,Sock}} | Resto]) ->
    gen_tcp:send(Sock, list_to_binary("GAME_START#" ++ User ++ "\n")),
    avisar_game_start(Resto).

%% ─────────────────────────────────────────────────────────────────
%% 10. GAME LOOP
%% ─────────────────────────────────────────────────────────────────
game_loop(GameState, Teclas, Objetos, TicksRestantes, ProxId, TopScoresPid, LobbyPid) ->
    receive
        tick ->
            if
                TicksRestantes =< 0 ->
                    terminar_jogo(GameState, TopScoresPid, LobbyPid);
                true ->
                    State1 = aplicar_fisica(GameState, Teclas),
                    {State2, Objetos2, ProxId2} = processar_colisoes_objetos(State1, Objetos, ProxId),
                    {State3, ProxId3}           = processar_capturas_jogadores(State2, ProxId2),
                    State4 = aplicar_repulsao_global(State3),
                    {Objetos3, ProxId4}         = garantir_pequeno(Objetos2, State4, ProxId3),
                    TempoRestante = (TicksRestantes * ?TICK_MS) div 1000,
                    broadcast_state(maps:to_list(State4), maps:to_list(State4), Objetos3, TempoRestante),
                    erlang:send_after(?TICK_MS, self(), tick),
                    game_loop(State4, Teclas, Objetos3, TicksRestantes - 1, ProxId4, TopScoresPid, LobbyPid)
            end;

        {key_press, User, Key} ->
            game_loop(GameState, atualizar_tecla(User, Key, true, Teclas),
                      Objetos, TicksRestantes, ProxId, TopScoresPid, LobbyPid);

        {key_release, User, Key} ->
            game_loop(GameState, atualizar_tecla(User, Key, false, Teclas),
                      Objetos, TicksRestantes, ProxId, TopScoresPid, LobbyPid);

        _ ->
            game_loop(GameState, Teclas, Objetos, TicksRestantes, ProxId, TopScoresPid, LobbyPid)
    end.

%% ─────────────────────────────────────────────────────────────────
%% 11. FÍSICA
%% ─────────────────────────────────────────────────────────────────
aplicar_fisica(GameState, Teclas) ->
    maps:map(fun(User, Estado) ->
        TeclasUser = maps:get(User, Teclas, #{left=>false, right=>false, forward=>false}),
        aplicar_fisica_jogador(Estado, TeclasUser)
    end, GameState).

aplicar_fisica_jogador({X, Y, Massa, Angulo, VX, VY, VAng, Score, Sock}, Teclas) ->
    AcelAngular = ?TORQUE / Massa,
    AcelLinear  = ?FORCA  / Massa,

    DeltaAng = case {maps:get(left, Teclas, false), maps:get(right, Teclas, false)} of
        {true, false} -> -AcelAngular;
        {false, true} ->  AcelAngular;
        _             ->  0.0
    end,
    NovoVAng   = (VAng + DeltaAng) * ?FRICCAO,
    NovoAngulo = Angulo + NovoVAng,

    {DVX, DVY} = case maps:get(forward, Teclas, false) of
        true  -> {AcelLinear * math:cos(NovoAngulo), AcelLinear * math:sin(NovoAngulo)};
        false -> {0.0, 0.0}
    end,
    NovoVX = (VX + DVX) * ?FRICCAO,
    NovoVY = (VY + DVY) * ?FRICCAO,

    Raio = raio(Massa),
    {NovoX, NovoVXf} = limitar(X + NovoVX, NovoVX, Raio, float(?WORLD_W) - Raio),
    {NovoY, NovoVYf} = limitar(Y + NovoVY, NovoVY, Raio, float(?WORLD_H) - Raio),
    {NovoX, NovoY, Massa, NovoAngulo, NovoVXf, NovoVYf, NovoVAng, Score, Sock}.

limitar(Pos, Vel, Min, Max) ->
    if Pos < Min -> {Min, 0.0};
       Pos > Max -> {Max, 0.0};
       true      -> {Pos, Vel}
    end.

raio(Massa)               -> math:sqrt(Massa / math:pi()).
distancia(X1,Y1,X2,Y2)   -> math:sqrt((X2-X1)*(X2-X1) + (Y2-Y1)*(Y2-Y1)).
posicao_aleatoria()       -> {float(rand:uniform(?WORLD_W - 80) + 40),
                              float(rand:uniform(?WORLD_H - 80) + 40)}.

%% ─────────────────────────────────────────────────────────────────
%% 12. COLISÕES JOGADOR <-> OBJETOS
%% Sobreposição (dist < raioJ + raioO):
%%   comestivel + jogador maior -> captura (ganha massa), repõe novo objeto
%%   venenoso -> objeto desaparece, jogador perde massa
%% ─────────────────────────────────────────────────────────────────
processar_colisoes_objetos(GameState, Objetos, ProxId) ->
    maps:fold(fun(User, {X,Y,Massa,Ang,VX,VY,VAng,Score,Sock}, {StateAcc, ObjsAcc, IdAcc}) ->
        RaioJ = raio(Massa),
        {Colididos, Restantes} = lists:partition(fun({_,OX,OY,OM,_}) ->
            distancia(X,Y,OX,OY) < (RaioJ + raio(OM) * 0.2)
        end, ObjsAcc),

        {NovaMassa, NovosObjs, NovoId} = lists:foldl(
            fun({_,_,_,OM,Tipo}, {MAcc, OAcc, IDAcc}) ->
                RaioO = raio(OM),
                case Tipo of
                    comestivel ->
                        if RaioJ >= RaioO ->
                            {MAcc + OM, [novo_objeto(IDAcc) | OAcc], IDAcc + 1};
                        true ->
                            %% objeto maior que jogador: não captura
                            {MAcc, OAcc, IDAcc}
                        end;
                    venenoso ->
                        NovaM = max(?MASSA_MINIMA, MAcc - OM),
                        {NovaM, OAcc, IDAcc}
                end
            end, {Massa, [], IdAcc}, Colididos),

        NovoEstado  = {X,Y,NovaMassa,Ang,VX,VY,VAng,Score,Sock},
        ObjsFinal   = Restantes ++ NovosObjs,
        {maps:put(User, NovoEstado, StateAcc), ObjsFinal, max(NovoId, IdAcc + 1)}
    end, {GameState, Objetos, ProxId}, GameState).

%% ─────────────────────────────────────────────────────────────────
%% 13. CAPTURAS JOGADOR <-> JOGADOR
%% A cobre B completamente (dist + RaioB <= RaioA, A > B):
%%   A ganha massa/4, score+1; B perde massa/4 e reaparece
%% ─────────────────────────────────────────────────────────────────
processar_capturas_jogadores(GameState, ProxId) ->
    Jogadores = maps:to_list(GameState),
    processar_pares(Jogadores, GameState, ProxId).

processar_pares([], GS, Id) -> {GS, Id};
processar_pares([{UA, _} | Resto], GS, Id) ->
    {GS2, Id2} = processar_contra(UA, Resto, GS, Id),
    processar_pares(Resto, GS2, Id2).

processar_contra(_UA, [], GS, Id) -> {GS, Id};
processar_contra(UA, [{UB, _} | Resto], GS, Id) ->
    case {maps:get(UA, GS, undefined), maps:get(UB, GS, undefined)} of
        {undefined, _} -> processar_contra(UA, Resto, GS, Id);
        {_, undefined} -> processar_contra(UA, Resto, GS, Id);
        {{XA,YA,MA,AngA,VXA,VYA,VAA,ScA,SockA},
         {XB,YB,MB,AngB,VXB,VYB,VAB,ScB,SockB}} ->
            RA = raio(MA), RB = raio(MB),
            D  = distancia(XA,YA,XB,YB),
            GS2 = if
                MA > MB andalso (D + RB * 0.2) =< RA ->
                    T = MB / 4.0,
                    {RX,RY} = posicao_aleatoria(),
                    GS#{ UA => {XA,YA,MA+T,AngA,VXA,VYA,VAA,ScA+1,SockA},
                         UB => {RX,RY,max(?MASSA_MINIMA,MB-T),AngB,0.0,0.0,0.0,ScB,SockB} };
                MB > MA andalso (D + RA * 0.2) =< RB ->
                    T = MA / 4.0,
                    {RX,RY} = posicao_aleatoria(),
                    GS#{ UA => {RX,RY,max(?MASSA_MINIMA,MA-T),AngA,0.0,0.0,0.0,ScA,SockA},
                         UB => {XB,YB,MB+T,AngB,VXB,VYB,VAB,ScB+1,SockB} };
                true -> GS
            end,
            processar_contra(UA, Resto, GS2, Id)
    end.

%% ─────────────────────────────────────────────────────────────────
%% 14. BROADCAST
%% STATE#tempo#players#objects
%%   players: user:x:y:massa:angulo:score|...
%%   objects: id:x:y:massa:tipo(c|v)|...  ou "none"
%% ─────────────────────────────────────────────────────────────────
broadcast_state([], _, _, _) -> ok;
broadcast_state([{_,{_,_,_,_,_,_,_,_,Sock}} | Resto], Todos, Objs, Tempo) ->
    gen_tcp:send(Sock, list_to_binary(formatar_state(Todos, Objs, Tempo))),
    broadcast_state(Resto, Todos, Objs, Tempo).

formatar_state(Jogadores, Objetos, Tempo) ->
    PJ = string:join([lists:flatten(io_lib:format("~s:~.1f:~.1f:~.1f:~.4f:~p",
            [U,X,Y,M,A,S])) || {U,{X,Y,M,A,_,_,_,S,_}} <- Jogadores], "|"),
    PO = case Objetos of
        [] -> "none";
        _  -> string:join([lists:flatten(io_lib:format("~p:~.1f:~.1f:~.1f:~s",
                [Id,X,Y,M, case T of comestivel->"c";venenoso->"v" end]))
              || {Id,X,Y,M,T} <- Objetos], "|")
    end,
    io_lib:format("STATE#~p#~s#~s~n", [Tempo, PJ, PO]).

%% ─────────────────────────────────────────────────────────────────
%% 15. TECLAS
%% ─────────────────────────────────────────────────────────────────
atualizar_tecla(User, Key, Valor, Teclas) ->
    TU = maps:get(User, Teclas, #{left=>false,right=>false,forward=>false}),
    maps:put(User, maps:put(key_atom(Key), Valor, TU), Teclas).

key_atom("LEFT")    -> left;
key_atom("RIGHT")   -> right;
key_atom("FORWARD") -> forward;
key_atom(_)         -> desconhecido.

%% ─────────────────────────────────────────────────────────────────
%% 16. FIM DE JOGO
%% ─────────────────────────────────────────────────────────────────
terminar_jogo(GameState, TopScoresPid, LobbyPid) ->
    io:format("GameRoom (~p): Fim de jogo!~n", [self()]),
    Jogadores = maps:to_list(GameState),
    {Vencedor, MaiorScore} = encontrar_vencedor(Jogadores),
    Empate = length([U || {U,{_,_,_,_,_,_,_,S,_}} <- Jogadores, S == MaiorScore]) > 1,
    if not Empate andalso MaiorScore > 0 ->
           TopScoresPid ! {add_score, Vencedor, MaiorScore};
       true -> ok
    end,
    Msg = if Empate -> "GAME_OVER#Empate! Partida ignorada no top.";
             true   -> "GAME_OVER#Vencedor: " ++ Vencedor ++
                       " com " ++ integer_to_list(MaiorScore) ++ " capturas!"
          end,
    lists:foreach(fun({_,{_,_,_,_,_,_,_,_,Sock}}) ->
        gen_tcp:send(Sock, list_to_binary(Msg ++ "\n"))
    end, Jogadores),
    LobbyPid ! {game_over}.

encontrar_vencedor(Jogadores) ->
    lists:foldl(fun({U,{_,_,_,_,_,_,_,S,_}}, {BU,BS}) ->
        if S > BS -> {U,S}; true -> {BU,BS} end
    end, {"", -1}, Jogadores).

%% ─────────────────────────────────────────────────────────────────
%% 17. AUXILIARES
%% ─────────────────────────────────────────────────────────────────
registar_jogadores([], _, Mapa) -> Mapa;
registar_jogadores([{User, _} | Resto], GamePid, Mapa) ->
    registar_jogadores(Resto, GamePid, maps:put(User, GamePid, Mapa)).


aplicar_repulsao_global(GameState) ->
    ListaJogadores = maps:to_list(GameState),
    todas_repulsoes(ListaJogadores, GameState).

todas_repulsoes([], GS) -> GS;
todas_repulsoes([{UA, _} | Resto], GS) ->
    GS2 = repulsao_contra_resto(UA, Resto, GS),
    todas_repulsoes(Resto, GS2).

repulsao_contra_resto(_UA, [], GS) -> GS;
repulsao_contra_resto(UA, [{UB, _} | Resto], GS) ->
    case {maps:get(UA, GS), maps:get(UB, GS)} of
        {{XA,YA,MA,AngA,VXA,VYA,VAA,ScA,SockA}, 
         {XB,YB,MB,AngB,VXB,VYB,VAB,ScB,SockB}} ->
            RA = raio(MA),
            RB = raio(MB),
            Dist = distancia(XA, YA, XB, YB),
            MinDist = RA + RB,

            DiferencaMassaSuficiente = (MA > MB * 1.2) orelse (MB > MA * 1.2),


            if (Dist < MinDist) andalso (Dist > 0) andalso (not DiferencaMassaSuficiente) ->
                %estao sobrepostos
                Overlap = MinDist - Dist,
                %direçao do empurrao
                DX = (XA - XB) / Dist,
                DY = (YA - YB) / Dist,

                %empurra cada um metade da sobreposiçao
                Forca = Overlap * 0.5,
                GS2 = GS#{
                    UA => {XA + DX * Forca, YA + DY * Forca, MA, AngA, VXA, VYA, VAA, ScA, SockA},
                    UB => {XB - DX * Forca, YB - DY * Forca, MB, AngB, VXB, VYB, VAB, ScB, SockB}
                },
                repulsao_contra_resto(UA, Resto, GS2);
            true ->
                repulsao_contra_resto(UA, Resto, GS)
            end;
        _ -> GS

    end.
