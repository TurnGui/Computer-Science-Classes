#include "golf_estado.h"
#include "golf.h"

#include <stdlib.h>

static void inicializar_topos(GolfEstado *e) { //topo de cada coluna 
    for (int c = 0; c < GOLF_COLUNAS; c++) {
        e->topo[c] = GOLF_LINHAS - 1;
    }
}

void golf_estado_inicializar(GolfEstado *e, unsigned seed) {
    srand(seed); //gerar seed random pra termos baralhos random

    baralho_inicializar(&e->stock); // cria baralho por ordem
    baralho_embaralhar(&e->stock); // baralha

    for (int linha = 0; linha < GOLF_LINHAS; linha++) { // desenhar a mesa 7 por 5
        for (int col = 0; col < GOLF_COLUNAS; col++) {
            e->mesa[col][linha] = baralho_retirar(&e->stock); //tira carta do stock e mete na mesa usando baralho retirar
        }
    }

    inicializar_topos(e); //topo de cada coluna que está na linha 4

    e->descarte = baralho_retirar(&e->stock); // tira uma carta do baralho (primeira a ser jogada) pra ser stock
}

int golf_stock_vazio(const GolfEstado *e) { // ver se stock acabou usando baralho vazio de baralho.c
    return baralho_vazio(&e->stock);
}

int golf_coluna_vazia(const GolfEstado *e, int coluna) {
    return e->topo[coluna] < 0; // topo de coluna vai de 4 até -1
}

Carta golf_topo_coluna(const GolfEstado *e, int coluna) {
    return e->mesa[coluna][e->topo[coluna]];  // vai a matrix mesa escolher o topo de uma coluna tipo e->mesa[2][3]
}

Carta golf_descarte(const GolfEstado *e) {
    return e->descarte; // devolve descarte
}

int golf_pode_jogar_coluna(const GolfEstado *e, int coluna) {
    if (coluna < 0 || coluna >= GOLF_COLUNAS) return 0; //paço de verificação pra ver se esta vazia a coluna ou coluna maior que permitida no jogo
    if (golf_coluna_vazia(e, coluna)) return 0;
    return golf_carta_pode_jogar(golf_topo_coluna(e, coluna), e->descarte); //buscar topo coluna e usar pode_jogar pra ver se podemos jogar
}

int golf_jogar_coluna(GolfEstado *e, int coluna) {
    if (!golf_pode_jogar_coluna(e, coluna)) return 0; //se nao for valido jogar aborta

    e->descarte = golf_topo_coluna(e, coluna); //descarta passa a ser o topo da coluna
    e->topo[coluna]--; // remover carta de coluna , coluna diminui

    return 1;
}

int golf_puxar_stock(GolfEstado *e) {  // sacar do stock pra descarte
    if (golf_stock_vazio(e)) return 0; // se vazio stock nao saca nada
    e->descarte = baralho_retirar(&e->stock); //descarte leva com a carta do stock usando o baralho retirar
    return 1;
}

int golf_ganhou(const GolfEstado *e) {
    for (int c = 0; c < GOLF_COLUNAS; c++) { 
        if (!golf_coluna_vazia(e, c)) return 0; //se todas as colunas vazias gg ganhamos
    }
    return 1;
}

int golf_tem_jogada(const GolfEstado *e) { 
    for (int c = 0; c < GOLF_COLUNAS; c++) {
        if (golf_pode_jogar_coluna(e, c)) return 1; // ve nas colunas todas se tem jogada
    }
    return 0;
}