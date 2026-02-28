#ifndef GOLF_ESTADO_H
#define GOLF_ESTADO_H

#include "baralho.h"
#include "carta.h"

#define GOLF_COLUNAS 7
#define GOLF_LINHAS  5

typedef struct {
    Carta mesa[GOLF_COLUNAS][GOLF_LINHAS]; // a mesa
    int topo[GOLF_COLUNAS];   // -1 se a coluna estiver vazia, senão entre 0 e 4 pq a coluna pode ter ate 5 cartas
    Carta descarte; // carta no topo do baralho
    Baralho stock;  // baralho
} GolfEstado;

void golf_estado_inicializar(GolfEstado *e, unsigned seed);

int golf_stock_vazio(const GolfEstado *e);
int golf_coluna_vazia(const GolfEstado *e, int coluna);

Carta golf_topo_coluna(const GolfEstado *e, int coluna);
Carta golf_descarte(const GolfEstado *e);

int golf_pode_jogar_coluna(const GolfEstado *e, int coluna);
int golf_jogar_coluna(GolfEstado *e, int coluna);

int golf_puxar_stock(GolfEstado *e);

int golf_ganhou(const GolfEstado *e);
int golf_tem_jogada(const GolfEstado *e);

#endif