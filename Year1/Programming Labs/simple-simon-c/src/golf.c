/**
 * @file golf.c
 * @brief Implementação das regras de jogabilidade do Golf Solitaire.
 */

#include "golf.h"

/**
 * @brief Verifica se uma carta pode ser jogada sobre a carta de descarte.
 *
 * A carta tem de ser a posição acima ou abaixo da atual,
 * logo a diferença dos valores tem de ser 1 ou -1.
 *
 * @param mesa    Carta no topo da coluna da mesa.
 * @param baralho Carta no topo do descarte.
 * @return 1 se pode jogar, 0 caso contrário.
 */
int golf_carta_pode_jogar(Carta mesa, Carta baralho) {

    int diferenca = mesa.valor - baralho.valor;

    if (diferenca == 1 || diferenca == -1) {
        return 1;
    }
    return 0;
}