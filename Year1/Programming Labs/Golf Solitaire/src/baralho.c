#include "baralho.h"
#include <stdlib.h>

void baralho_inicializar(Baralho *b) { //*b para modificar o baralho original senão mexiamos numa copia, se andasse a passar valor iamos copiar 52 cartas a cada chamada
    
    int indice = 0;

    for (int naipe = 0; naipe < 4; naipe++) {
        for (int valor = 1; valor <= 13; valor++) {
            b->cartas[indice] = carta_criar(naipe, valor);
            indice++;
        }
    }

    b->topo = 0;
}

Carta baralho_retirar(Baralho *b) {
    Carta c = b->cartas[b->topo]; //vai a lista cartas e saca o topo
    b->topo++;
    return c;
}

int baralho_vazio(Baralho *b) { //importante pq o jogo termina quando o baralho termina
    return b->topo >= TAMANHO_BARALHO;
}

void baralho_embaralhar(Baralho *b) {
    for (int i = TAMANHO_BARALHO - 1; i > 0; i--) {
        int j = rand() % (i + 1); // vi de um men qq do github mas o rand() dá um numero random e tranforma num numero entre 0 e i

        Carta tmp = b->cartas[i]; // guarda posição atual
        b->cartas[i] = b->cartas[j]; // mete a carta random
        b->cartas[j] = tmp; // guarda a original
    }

    b->topo = 0; // volta a meter o topo em 0.
}