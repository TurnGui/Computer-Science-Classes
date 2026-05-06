/**
 * @file golf_estado.c
 * @brief Implementação das operações sobre o estado do Golf Solitaire.
 */

#include "golf_estado.h"
#include "golf.h"

#include <stdlib.h>

/**
 * @brief Inicializa o topo de cada coluna para a última linha (GOLF_LINHAS - 1).
 *
 * Função auxiliar estática — não faz parte da API pública.
 *
 * @param e Apontador para o estado do jogo.
 */
static void inicializar_topos(GolfEstado *e) {
    for (int c = 0; c < GOLF_COLUNAS; c++) {
        e->topo[c] = GOLF_LINHAS - 1;
    }
}

/**
 * @brief Inicializa o estado do jogo com uma semente aleatória.
 *
 * Gera seed random para termos baralhos diferentes em cada jogo.
 * Cria o baralho por ordem, embaralha, distribui 35 cartas pela mesa
 * (7 colunas × 5 linhas) e retira a primeira carta para o descarte.
 *
 * @param e    Apontador para o estado a inicializar.
 * @param seed Semente para o gerador pseudo-aleatório (srand).
 */
void golf_estado_inicializar(GolfEstado *e, unsigned seed) {
    srand(seed);

    baralho_inicializar(&e->stock);
    baralho_embaralhar(&e->stock);

    for (int linha = 0; linha < GOLF_LINHAS; linha++) {
        for (int col = 0; col < GOLF_COLUNAS; col++) {
            e->mesa[col][linha] = baralho_retirar(&e->stock);
        }
    }

    inicializar_topos(e);

    e->descarte = baralho_retirar(&e->stock);
}

/**
 * @brief Verifica se o stock acabou usando baralho_vazio de baralho.c.
 *
 * @param e Apontador constante para o estado do jogo.
 * @return 1 se o stock está vazio, 0 caso contrário.
 */
int golf_stock_vazio(const GolfEstado *e) {
    return baralho_vazio(&e->stock);
}

/**
 * @brief Verifica se uma coluna da mesa está vazia.
 *
 * O topo de coluna vai de 4 até -1, sendo -1 coluna vazia.
 *
 * @param e      Apontador constante para o estado do jogo.
 * @param coluna Índice da coluna (0 a GOLF_COLUNAS-1).
 * @return 1 se a coluna está vazia, 0 caso contrário.
 */
int golf_coluna_vazia(const GolfEstado *e, int coluna) {
    return e->topo[coluna] < 0;
}

/**
 * @brief Devolve a carta no topo de uma coluna.
 *
 * Vai à matriz mesa escolher o topo de uma coluna, tipo e->mesa[2][3].
 *
 * @param e      Apontador constante para o estado do jogo.
 * @param coluna Índice da coluna (0 a GOLF_COLUNAS-1).
 * @return Carta no topo da coluna indicada.
 */
Carta golf_topo_coluna(const GolfEstado *e, int coluna) {
    return e->mesa[coluna][e->topo[coluna]];
}

/**
 * @brief Devolve a carta actualmente no topo do descarte.
 *
 * @param e Apontador constante para o estado do jogo.
 * @return Carta de descarte actual.
 */
Carta golf_descarte(const GolfEstado *e) {
    return e->descarte;
}

/**
 * @brief Verifica se é possível jogar a carta do topo de uma coluna.
 *
 * Passo de verificação para ver se a coluna está vazia ou fora do intervalo,
 * depois usa golf_carta_pode_jogar para ver se podemos jogar.
 *
 * @param e      Apontador constante para o estado do jogo.
 * @param coluna Índice da coluna a testar (0 a GOLF_COLUNAS-1).
 * @return 1 se a jogada é válida, 0 caso contrário.
 */
int golf_pode_jogar_coluna(const GolfEstado *e, int coluna) {
    if (coluna < 0 || coluna >= GOLF_COLUNAS) return 0;
    if (golf_coluna_vazia(e, coluna)) return 0;
    return golf_carta_pode_jogar(golf_topo_coluna(e, coluna), e->descarte);
}

/**
 * @brief Executa a jogada da carta do topo de uma coluna para o descarte.
 *
 * O descarte passa a ser o topo da coluna e a coluna diminui.
 * Se a jogada não for válida aborta.
 *
 * @param e      Apontador para o estado do jogo.
 * @param coluna Índice da coluna a jogar (0 a GOLF_COLUNAS-1).
 * @return 1 se a jogada foi efectuada, 0 caso contrário.
 */
int golf_jogar_coluna(GolfEstado *e, int coluna) {
    if (!golf_pode_jogar_coluna(e, coluna)) return 0;

    e->descarte = golf_topo_coluna(e, coluna);
    e->topo[coluna]--;

    return 1;
}

/**
 * @brief Retira uma carta do stock e coloca-a no descarte.
 *
 * Se o stock estiver vazio não saca nada.
 *
 * @param e Apontador para o estado do jogo.
 * @return 1 se a operação foi bem-sucedida, 0 se o stock estava vazio.
 */
int golf_puxar_stock(GolfEstado *e) {
    if (golf_stock_vazio(e)) return 0;
    e->descarte = baralho_retirar(&e->stock);
    return 1;
}

/**
 * @brief Verifica se o jogador ganhou (todas as colunas vazias).
 *
 * Se todas as colunas estiverem vazias, gg ganhamos.
 *
 * @param e Apontador constante para o estado do jogo.
 * @return 1 se ganhou, 0 caso contrário.
 */
int golf_ganhou(const GolfEstado *e) {
    for (int c = 0; c < GOLF_COLUNAS; c++) {
        if (!golf_coluna_vazia(e, c)) return 0;
    }
    return 1;
}

/**
 * @brief Verifica se existe pelo menos uma jogada possível na mesa.
 *
 * Vê em todas as colunas se tem jogada.
 *
 * @param e Apontador constante para o estado do jogo.
 * @return 1 se existe alguma jogada válida, 0 caso contrário.
 */
int golf_tem_jogada(const GolfEstado *e) {
    for (int c = 0; c < GOLF_COLUNAS; c++) {
        if (golf_pode_jogar_coluna(e, c)) return 1;
    }
    return 0;
}