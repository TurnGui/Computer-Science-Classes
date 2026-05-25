/**
 * @file simon_regras.h
 * @brief Regras de jogabilidade do Simple Simon.
 *
 * Verifica as condições de movimento entre pilhas:
 * sequências válidas, mesmo naipe, valor adjacente, pilha vazia.
 */

#ifndef SIMON_REGRAS_H
#define SIMON_REGRAS_H

#include "pilha.h"

/**
 * @brief Verifica se duas cartas são do mesmo naipe.
 * @param a Primeira carta.
 * @param b Segunda carta.
 * @return 1 se mesmo naipe, 0 caso contrário.
 */
int simon_mesmo_naipe(Carta a, Carta b);

/**
 * @brief Verifica se a carta 'baixo' tem valor imediatamente inferior a 'cima'.
 * @param cima Carta de destino (topo do destino).
 * @param baixo Carta de origem (topo da sequência a mover).
 * @return 1 se válido, 0 caso contrário.
 */
int simon_valor_inferior(Carta cima, Carta baixo);

/**
 * @brief Verifica se uma sequência do topo de uma pilha é válida para mover.
 *
 * Uma sequência válida tem n cartas contíguas do topo, do mesmo naipe,
 * em ordem decrescente de valor (cada carta tem valor = carta abaixo - 1).
 *
 * @param p      Pilha de origem.
 * @param n      Número de cartas a mover (a partir do topo).
 * @return 1 se a sequência é válida, 0 caso contrário.
 */
int simon_sequencia_valida(const Pilha *p, int n);

/**
 * @brief Verifica se é possível mover n cartas de origem para destino.
 *
 * Regras do Simple Simon (disjunção das MOV):
 * - 1 carta com valor imediatamente inferior ao topo do destino
 * - sequência do mesmo naipe decrescente com valor inferior ao destino
 * - qualquer carta/sequência para coluna vazia
 *
 * @param origem  Pilha de onde se move.
 * @param destino Pilha para onde se move.
 * @param n       Número de cartas a mover.
 * @return 1 se o movimento é válido, 0 caso contrário.
 */
int simon_pode_mover(const Pilha *origem, const Pilha *destino, int n);

/**
 * @brief Verifica se uma pilha contém uma sequência completa K..A do mesmo naipe.
 *
 * Usado para o AUTO: sequência de 13 cartas do mesmo naipe, de Rei (13) a Ás (1).
 *
 * @param p Pilha a verificar.
 * @return 1 se contém sequência completa no topo, 0 caso contrário.
 */
int simon_sequencia_completa(const Pilha *p);

#endif
