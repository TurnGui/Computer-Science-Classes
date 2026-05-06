/**
 * @file simon_estado.c
 * @brief Implementação das operações sobre o estado do Simple Simon.
 */

#include "simon_estado.h"
#include "simon_regras.h"
#include <stdlib.h>

/**
 * @brief percorre as 10 TAB e 4 FUND e chama pilha_inicializar em cada uma. 
 * Função auxiliar para manter simon_estado_inicializar dentro do limite de statements.
 */
static void inicializar_pilhas(SimonEstado *e) {
    for (int i = 0; i < SIMON_COLUNAS; i++) pilha_inicializar(&e->tab[i]);
    for (int i = 0; i < SIMON_FUNDACOES; i++) pilha_inicializar(&e->fund[i]);
}

/**
 * @brief percorre as 10 colunas e para cada coluna faz push do número de cartas indicado em SIMON_INIT_TAMANHOS, 
 * retirando-as do baralho com baralho_retirar.
 */
static void distribuir_cartas(SimonEstado *e, Baralho *b) {
    for (int col = 0; col < SIMON_COLUNAS; col++) {
        for (int j = 0; j < SIMON_INIT_TAMANHOS[col]; j++) {
            pilha_push(&e->tab[col], baralho_retirar(b));
        }
    }
}

/**
 * @brief Chama srand com a seed, cria um baralho local, inicializa-o e embaralha-o. Depois chama duas funções auxiliares:
 * inicializar_pilhas que põe todas as TAB e FUND vazias, e distribuir_cartas que vai buscar cartas ao baralho 
 * e faz push para cada coluna segundo os tamanhos definidos em SIMON_INIT_TAMANHOS (8,8,8,7,6,5,4,3,2,1).
 */
void simon_estado_inicializar(SimonEstado *e, unsigned seed) {
    srand(seed);
    Baralho b;
    baralho_inicializar(&b);
    baralho_embaralhar(&b);
    inicializar_pilhas(e);
    distribuir_cartas(e, &b);
}

/**
 * @brief percorre as 10 TAB e verifica se todas estão vazias. Se alguma não estiver devolve 0, se todas estiverem devolve 1.
 */
int simon_ganhou(const SimonEstado *e) {
    for (int i = 0; i < SIMON_COLUNAS; i++) {
        if (!pilha_vazia(&e->tab[i])) return 0;
    }
    return 1;
}

/**
 * @brief verifica se origem e destino estão entre 0 e 9 e são diferentes entre si.
 */
static int indices_validos(int origem, int dest) {
    return origem >= 0 && origem < SIMON_COLUNAS
        && dest >= 0 && dest < SIMON_COLUNAS
        && origem != dest;
}

/**
 * @brief verifica se um movimento é válido. Primeiro chama indices_validos para garantir que origem e destino existem e são diferentes, 
 * depois chama simon_pode_mover com as pilhas correspondentes.
 */
int simon_pode_jogar(const SimonEstado *e, int origem, int dest, int n) {
    if (!indices_validos(origem, dest)) return 0;
    return simon_pode_mover(&e->tab[origem], &e->tab[dest], n);
}

/**
 * @brief usa um array temporário tmp para mover as n cartas. Faz pop das n cartas de origem para o tmp, 
 * e depois push do tmp para o destino em ordem inversa, para preservar a ordem original da sequência.
 */
static void mover_cartas(Pilha *origem, Pilha *destino, int n) {
    Carta tmp[MAX_CARTAS_PILHA];
    for (int i = 0; i < n; i++) tmp[i] = pilha_pop(origem);
    for (int i = n - 1; i >= 0; i--) pilha_push(destino, tmp[i]);
}

/**
 * @brief Executa o movimento de n cartas de origem para destino.
 */
int simon_jogar(SimonEstado *e, int origem, int dest, int n) {
    if (!simon_pode_jogar(e, origem, dest, n)) return 0;
    mover_cartas(&e->tab[origem], &e->tab[dest], n);
    return 1;
}

/**
 * @brief verifica se a coluna tem uma sequência completa com simon_sequencia_completa,
 *  e se sim procura uma fundação vazia e move as 13 cartas para lá com mover_cartas.
 */
static int auto_uma_coluna(SimonEstado *e, int col) {
    if (!simon_sequencia_completa(&e->tab[col])) return 0;
    for (int f = 0; f < SIMON_FUNDACOES; f++) {
        if (!pilha_vazia(&e->fund[f])) continue;
        mover_cartas(&e->tab[col], &e->fund[f], 13);
        return 1;
    }
    return 0;
}

/**
 * @brief executa os movimentos automáticos. Corre num loop enquanto encontrar sequências completas K..A do mesmo naipe em alguma TAB. 
 * Para cada coluna chama auto_uma_coluna.
 */
int simon_auto(SimonEstado *e) {
    int total = 0;
    int encontrou = 1;
    while (encontrou) {
        encontrou = 0;
        for (int col = 0; col < SIMON_COLUNAS; col++) {
            if (auto_uma_coluna(e, col)) { encontrou = 1; total++; }
        }
    }
    return total;
}

/**
 * @brief percorre todas as combinações de origem e destino (10×10) e verifica com simon_pode_mover se existe 
 * pelo menos um movimento válido de 1 carta. Se encontrar algum devolve 1, se não encontrar nenhum devolve 0.
 */
int simon_tem_jogada(const SimonEstado *e) {
    for (int o = 0; o < SIMON_COLUNAS; o++) {
        if (pilha_vazia(&e->tab[o])) continue;
        for (int d = 0; d < SIMON_COLUNAS; d++) {
            if (o == d) continue;
            if (simon_pode_mover(&e->tab[o], &e->tab[d], 1)) return 1;
        }
    }
    return 0;
}
