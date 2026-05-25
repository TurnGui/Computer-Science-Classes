/**
 * @file pilha.h
 * @brief Estrutura Pilha genérica de cartas e operações sobre ela.
 *
 * Uma pilha é um array fixo de cartas com um índice de topo.
 * Usada para representar colunas do tabuleiro e fundações.
 */

#ifndef PILHA_H
#define PILHA_H

#include "carta.h"

/** @brief Número máximo de cartas numa pilha. */
#define MAX_CARTAS_PILHA 52

/**
 * @brief Estrutura que representa uma pilha de cartas.
 *
 * @var Pilha::cartas  Array de cartas da pilha.
 * @var Pilha::topo    Índice do topo (-1 = vazia).
 */
typedef struct {
    Carta cartas[MAX_CARTAS_PILHA];
    int topo;
} Pilha;

/**
 * @brief Inicializa uma pilha vazia.
 * @param p Apontador para a pilha.
 */
void pilha_inicializar(Pilha *p);

/**
 * @brief Verifica se a pilha está vazia.
 * @param p Apontador constante para a pilha.
 * @return 1 se vazia, 0 caso contrário.
 */
int pilha_vazia(const Pilha *p);

/**
 * @brief Verifica se a pilha está cheia.
 * @param p Apontador constante para a pilha.
 * @return 1 se cheia, 0 caso contrário.
 */
int pilha_cheia(const Pilha *p);

/**
 * @brief Devolve o número de cartas na pilha.
 * @param p Apontador constante para a pilha.
 * @return Número de cartas (0 se vazia).
 */
int pilha_tamanho(const Pilha *p);

/**
 * @brief Devolve a carta no topo da pilha sem a remover.
 * @param p Apontador constante para a pilha.
 * @return Carta no topo. Comportamento indefinido se vazia.
 */
Carta pilha_topo(const Pilha *p);

/**
 * @brief Devolve a carta na posição indicada (0 = fundo).
 * @param p   Apontador constante para a pilha.
 * @param pos Posição (0 = fundo, topo = tamanho-1).
 * @return Carta na posição indicada.
 */
Carta pilha_carta(const Pilha *p, int pos);

/**
 * @brief Adiciona uma carta ao topo da pilha.
 * @param p Apontador para a pilha.
 * @param c Carta a adicionar.
 * @return 1 se bem-sucedido, 0 se a pilha está cheia.
 */
int pilha_push(Pilha *p, Carta c);

/**
 * @brief Remove e devolve a carta do topo da pilha.
 * @param p Apontador para a pilha.
 * @return Carta removida. Comportamento indefinido se vazia.
 */
Carta pilha_pop(Pilha *p);

#endif
