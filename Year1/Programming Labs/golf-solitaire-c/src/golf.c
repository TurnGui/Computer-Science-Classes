#include "golf.h"

int golf_carta_pode_jogar(Carta mesa, Carta baralho) {

    int diferenca = mesa.valor - baralho.valor; //a carta tem de ser a posição acima ou abaixo da atual

    if (diferenca == 1 || diferenca == -1) { // logo a diferença da pos das duas tem de ser 1 ou -1
        return 1; // VDD
    }
 return 0;
}