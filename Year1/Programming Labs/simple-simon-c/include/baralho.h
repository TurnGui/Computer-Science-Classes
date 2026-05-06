#ifndef BARALHO_H
#define BARALHO_H

#include "carta.h"

#define TAMANHO_BARALHO 52

typedef struct {
    Carta cartas[TAMANHO_BARALHO]; //baralho é uma lista de 52 cartas
    int topo;  // índice da próxima carta a retirar
} Baralho;

// Inicializa o baralho com 52 cartas e ele vai diminuindo de tamanho ao longo do jogo
void baralho_inicializar(Baralho *b);

// Retira uma carta do baralho
Carta baralho_retirar(Baralho *b);

// Verifica se o baralho está vazio pq o jogo termina se vazio
int baralho_vazio(const Baralho *b);


void baralho_embaralhar(Baralho *b);

#endif