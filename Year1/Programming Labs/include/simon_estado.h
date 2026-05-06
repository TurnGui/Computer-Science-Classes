/**
 * @file simon_estado.h
 * @brief Estado completo de uma partida de Simple Simon e operações sobre ele.
 *
 * O estado agrega 10 colunas de tabuleiro (TAB) e 4 fundações (FUND).
 */

#ifndef SIMON_ESTADO_H
#define SIMON_ESTADO_H

#include "pilha.h"
#include "baralho.h"

/** @brief Número de colunas do tabuleiro. */
#define SIMON_COLUNAS 10

/** @brief Número de fundações. */
#define SIMON_FUNDACOES 4

/**
 * @brief Tamanhos iniciais das 10 colunas do Simple Simon.
 *
 * Colunas 0-2: 8 cartas, 3: 7, 4: 6, 5: 5, 6: 4, 7: 3, 8: 2, 9: 1.
 */
static const int SIMON_INIT_TAMANHOS[SIMON_COLUNAS] = {8,8,8,7,6,5,4,3,2,1};

/**
 * @brief Estrutura que representa o estado completo do Simple Simon.
 *
 * @var SimonEstado::tab       Colunas do tabuleiro (10 pilhas).
 * @var SimonEstado::fund      Fundações (4 pilhas).
 */
typedef struct {
    Pilha tab[SIMON_COLUNAS];
    Pilha fund[SIMON_FUNDACOES];
} SimonEstado;

/**
 * @brief Inicializa o estado do jogo com uma semente aleatória.
 * @param e    Apontador para o estado.
 * @param seed Semente para o gerador pseudo-aleatório.
 */
void simon_estado_inicializar(SimonEstado *e, unsigned seed);

/**
 * @brief Verifica se o jogador ganhou (todas as TAB vazias).
 * @param e Apontador constante para o estado.
 * @return 1 se ganhou, 0 caso contrário.
 */
int simon_ganhou(const SimonEstado *e);

/**
 * @brief Verifica se um movimento de n cartas de uma TAB para outra é válido.
 * @param e      Apontador constante para o estado.
 * @param origem Índice da coluna de origem (0 a SIMON_COLUNAS-1).
 * @param dest   Índice da coluna de destino (0 a SIMON_COLUNAS-1).
 * @param n      Número de cartas a mover.
 * @return 1 se válido, 0 caso contrário.
 */
int simon_pode_jogar(const SimonEstado *e, int origem, int dest, int n);

/**
 * @brief Executa o movimento de n cartas de uma TAB para outra.
 * @param e      Apontador para o estado.
 * @param origem Índice da coluna de origem.
 * @param dest   Índice da coluna de destino.
 * @param n      Número de cartas a mover.
 * @return 1 se executado, 0 caso contrário.
 */
int simon_jogar(SimonEstado *e, int origem, int dest, int n);

/**
 * @brief Executa movimentos automáticos (AUTO): sequências K..A para fundações vazias.
 * @param e Apontador para o estado.
 * @return Número de movimentos automáticos realizados.
 */
int simon_auto(SimonEstado *e);

/**
 * @brief Verifica se existe alguma jogada possível no estado actual.
 * @param e Apontador constante para o estado.
 * @return 1 se existe jogada, 0 caso contrário.
 */
int simon_tem_jogada(const SimonEstado *e);

#endif
