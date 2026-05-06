/**
 * @file main.c
 * @brief Interface de linha de comandos (CLI) do Simple Simon.
 *
 * Trata da apresentação do estado do jogo e da leitura de comandos do jogador.
 * Separado da lógica — só chama funções de simon_estado.h.
 */

#include <stdio.h>
#include <locale.h>
#include <time.h>
#include <wchar.h>

#include "carta.h"
#include "simon_estado.h"

/**
 * @brief Imprime o símbolo Unicode da carta seguido de espaço.
 * @param c Carta a imprimir.
 */
static void imprimir_carta(Carta c) {
    wprintf(L"%lc ", carta_unicode(c));
}

/**
 * @brief Imprime dois espaços em branco quando não há carta naquele lugar.
 */
static void imprimir_espaco(void) {
    wprintf(L"  ");
}

/**
 * @brief Imprime as 4 fundações no topo do ecrã.
 *
 * Mostra "[ ]" se a fundação estiver vazia, ou a carta do topo se tiver cartas.
 *
 * @param e Apontador constante para o estado do jogo.
 */
static void ui_mostrar_fundacoes(const SimonEstado *e) {
    wprintf(L"Fundações: ");
    for (int f = 0; f < SIMON_FUNDACOES; f++) {
        if (pilha_vazia(&e->fund[f])) wprintf(L"[ ] ");
        else imprimir_carta(pilha_topo(&e->fund[f]));
    }
    wprintf(L"\n\n");
}

/**
 * @brief Imprime uma linha do tabuleiro para todas as colunas.
 *
 * Calcula a posição real da carta em cada coluna com base na linha atual
 * e no número máximo de linhas. Se não houver carta nessa posição imprime espaço.
 *
 * @param e          Apontador constante para o estado do jogo.
 * @param linha      Índice da linha atual a imprimir.
 * @param max_linhas Número máximo de linhas do tabuleiro.
 */
static void ui_mostrar_linha(const SimonEstado *e, int linha, int max_linhas) {
    for (int col = 0; col < SIMON_COLUNAS; col++) {
        int pos = pilha_tamanho(&e->tab[col]) - max_linhas + linha;
        if (pos >= 0 && pos < pilha_tamanho(&e->tab[col]))
            imprimir_carta(pilha_carta(&e->tab[col], pos));
        else
            imprimir_espaco();
    }
    wprintf(L"\n");
}

/**
 * @brief Imprime o tabuleiro completo com os números das colunas no topo.
 *
 * Calcula a coluna com mais cartas para determinar quantas linhas imprimir,
 * depois chama ui_mostrar_linha para cada linha.
 *
 * @param e Apontador constante para o estado do jogo.
 */
static void ui_mostrar_tabuleiro(const SimonEstado *e) {
    int max_linhas = 0;
    for (int col = 0; col < SIMON_COLUNAS; col++) {
        int t = pilha_tamanho(&e->tab[col]);
        if (t > max_linhas) max_linhas = t;
    }
    wprintf(L" 0  1  2  3  4  5  6  7  8  9\n");
    for (int linha = 0; linha < max_linhas; linha++)
        ui_mostrar_linha(e, linha, max_linhas);
}

/**
 * @brief Imprime o estado completo do jogo: fundações, tabuleiro e comandos.
 * @param e Apontador constante para o estado do jogo.
 */
static void ui_mostrar(const SimonEstado *e) {
    wprintf(L"\n");
    ui_mostrar_fundacoes(e);
    ui_mostrar_tabuleiro(e);
    wprintf(L"\nComandos: m <orig> <dest> <n> (mover), q (sair)\n");
}

/**
 * @brief Lê um comando do teclado no formato "<letra> [orig] [dest] [n]".
 *
 * Usa fgets para evitar deixar '\\n' no buffer e lidar com inputs inválidos.
 *
 * @param cmd Apontador para guardar a letra do comando.
 * @param a   Apontador para a coluna de origem.
 * @param b   Apontador para a coluna de destino.
 * @param n   Apontador para o número de cartas a mover (default 1).
 * @return 1 se leu com sucesso, 0 se falhou.
 */
static int ler_comando(char *cmd, int *a, int *b, int *n) {
    char linha[64];
    if (!fgets(linha, sizeof(linha), stdin)) return 0;
    *cmd = '\0'; *a = -1; *b = -1; *n = 1;
    if (sscanf(linha, " %c %d %d %d", cmd, a, b, n) >= 1) return 1;
    return 0;
}

/**
 * @brief Verifica se o jogo terminou — vitória ou sem jogadas possíveis.
 * @param e Apontador constante para o estado do jogo.
 * @return 1 se o jogo terminou, 0 caso contrário.
 */
static int jogo_terminou(const SimonEstado *e) {
    return simon_ganhou(e) || !simon_tem_jogada(e);
}

/**
 * @brief Imprime a mensagem de vitória ou derrota no final do jogo.
 * @param e Apontador constante para o estado do jogo.
 */
static void mostrar_resultado(const SimonEstado *e) {
    if (simon_ganhou(e)) wprintf(L"\nParabéns, ganhou! :)\n");
    else wprintf(L"\nSem jogadas possíveis. Perdeu :(\n");
}

/**
 * @brief Comando 'm' — move n cartas de uma coluna para outra.
 *
 * Chama simon_jogar e se tiver sucesso corre os movimentos automáticos.
 *
 * @param e    Apontador para o estado do jogo.
 * @param orig Coluna de origem.
 * @param dest Coluna de destino.
 * @param n    Número de cartas a mover.
 * @return 1 para continuar o jogo.
 */
static int cmd_mover(SimonEstado *e, int orig, int dest, int n) {
    if (!simon_jogar(e, orig, dest, n)) wprintf(L"Jogada inválida.\n");
    else simon_auto(e);
    return 1;
}

/**
 * @brief Comando 'q' — sai do jogo.
 * @return 0 para terminar o loop.
 */
static int cmd_sair(SimonEstado *e, int a, int b, int n) {
    (void)e; (void)a; (void)b; (void)n;
    return 0;
}

/**
 * @brief Comando inválido — imprime erro e continua o jogo.
 * @return 1 para continuar o jogo.
 */
static int cmd_invalido(SimonEstado *e, int a, int b, int n) {
    (void)e; (void)a; (void)b; (void)n;
    wprintf(L"Comando inválido.\n");
    return 1;
}

/** @brief Tipo de função de comando. */
typedef int (*CmdFn)(SimonEstado *, int, int, int);

/**
 * @brief Estrutura que associa uma letra a uma função de comando.
 * @var EntradaCmd::letra  Letra do comando.
 * @var EntradaCmd::funcao Função a executar.
 */
typedef struct { char letra; CmdFn funcao; } EntradaCmd;

/** @brief Lookup table que associa letras às funções de comando. */
static const EntradaCmd comandos[] = {
    { 'm', cmd_mover },
    { 'q', cmd_sair  },
};

/**
 * @brief Procura a função correspondente à letra do comando na lookup table.
 *
 * Se não encontrar devolve cmd_invalido.
 *
 * @param letra Letra do comando introduzido pelo jogador.
 * @return Função de comando correspondente.
 */
static CmdFn procurar_comando(char letra) {
    int n = sizeof(comandos) / sizeof(comandos[0]);
    for (int i = 0; i < n; i++) {
        if (comandos[i].letra == letra) return comandos[i].funcao;
    }
    return cmd_invalido;
}

/**
 * @brief Ponto de entrada do programa.
 *
 * Ativa unicode no terminal, inicializa o estado com seed aleatória,
 * corre os automáticos iniciais e entra no loop principal do jogo
 * usando um while para evitar breaks.
 *
 * @return 0 ao terminar.
 */
int main(void) {
    setlocale(LC_ALL, "");
    SimonEstado e;
    simon_estado_inicializar(&e, (unsigned)time(NULL));
    simon_auto(&e);

    char cmd = '\0';
    int orig = -1, dest = -1, n = 1;

    ui_mostrar(&e);
    wprintf(L"> ");

    while (ler_comando(&cmd, &orig, &dest, &n)
           && procurar_comando(cmd)(&e, orig, dest, n)
           && !jogo_terminou(&e)) {
        ui_mostrar(&e);
        wprintf(L"> ");
    }

    mostrar_resultado(&e);
    return 0;
}
