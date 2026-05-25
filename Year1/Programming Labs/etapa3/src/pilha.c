/**
 * @file pilha.c
 * @brief Implementação das operações sobre Pilha.
 */

#include "pilha.h"

/**
 * @brief Inicializa uma pilha vazia.
 */
void pilha_inicializar(Pilha *p) {
    p->topo = -1;
}

/**
 * @brief Verifica se a pilha está vazia.
 */
int pilha_vazia(const Pilha *p) {
    return p->topo < 0;
}

/**
 * @brief Verifica se a pilha está cheia.
 */
int pilha_cheia(const Pilha *p) {
    return p->topo >= MAX_CARTAS_PILHA - 1;
}

/**
 * @brief Devolve o número de cartas na pilha.
 */
int pilha_tamanho(const Pilha *p) {
    return p->topo + 1;
}

/**
 * @brief Devolve a carta no topo da pilha sem a remover.
 */
Carta pilha_topo(const Pilha *p) {
    return p->cartas[p->topo];
}

/**
 * @brief Devolve a carta na posição indicada (0 = fundo).
 */
Carta pilha_carta(const Pilha *p, int pos) {
    return p->cartas[pos];
}

/**
 * @brief Adiciona uma carta ao topo da pilha.
 */
int pilha_push(Pilha *p, Carta c) {
    if (pilha_cheia(p)) return 0;
    p->topo++;
    p->cartas[p->topo] = c;
    return 1;
}

/**
 * @brief Remove e devolve a carta do topo da pilha.
 */
Carta pilha_pop(Pilha *p) {
    Carta c = p->cartas[p->topo];
    p->topo--;
    return c;
}
