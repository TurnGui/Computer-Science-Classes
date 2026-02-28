#include "golf_estado.h"
#include "golf.h"

#include <stdlib.h>

static void inicializar_topos(GolfEstado *e) {
    for (int c = 0; c < GOLF_COLUNAS; c++) {
        e->topo[c] = GOLF_LINHAS - 1;
    }
}

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

int golf_stock_vazio(const GolfEstado *e) {
    return baralho_vazio((Baralho *)&e->stock);
}

int golf_coluna_vazia(const GolfEstado *e, int coluna) {
    return e->topo[coluna] < 0;
}

Carta golf_topo_coluna(const GolfEstado *e, int coluna) {
    return e->mesa[coluna][e->topo[coluna]];
}

Carta golf_descarte(const GolfEstado *e) {
    return e->descarte;
}

int golf_pode_jogar_coluna(const GolfEstado *e, int coluna) {
    if (coluna < 0 || coluna >= GOLF_COLUNAS) return 0;
    if (golf_coluna_vazia(e, coluna)) return 0;
    return golf_carta_pode_jogar(golf_topo_coluna(e, coluna), e->descarte);
}

int golf_jogar_coluna(GolfEstado *e, int coluna) {
    if (!golf_pode_jogar_coluna(e, coluna)) return 0;

    e->descarte = golf_topo_coluna(e, coluna);
    e->topo[coluna]--;

    return 1;
}

int golf_puxar_stock(GolfEstado *e) {
    if (golf_stock_vazio(e)) return 0;
    e->descarte = baralho_retirar(&e->stock);
    return 1;
}

int golf_ganhou(const GolfEstado *e) {
    for (int c = 0; c < GOLF_COLUNAS; c++) {
        if (!golf_coluna_vazia(e, c)) return 0;
    }
    return 1;
}

int golf_tem_jogada(const GolfEstado *e) {
    for (int c = 0; c < GOLF_COLUNAS; c++) {
        if (golf_pode_jogar_coluna(e, c)) return 1;
    }
    return 0;
}