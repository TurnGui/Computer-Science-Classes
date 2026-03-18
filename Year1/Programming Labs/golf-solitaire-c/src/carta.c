#include "carta.h"

Carta carta_criar(int naipe, int valor) {
    Carta c;
    c.naipe = naipe;
    c.valor = valor;
    return c;
}

int carta_valida(Carta c) {
    return (c.naipe >= 0 && c.naipe < 4 && c.valor >= 1 && c.valor <= 13); // so temos 4 naipes e cada naipe tem 13 cartas
}
 
// offsets unicode por valor. posição 12 (cavaleiro) saltamos
static const int offset_valor[14] = { //isto não conta como var global ! é uma constante do ficheiro
    0,   //índice 0 não usado
    1,   //A 
    2,
    3,
    4,
    5,
    6,
    7,
    8,
    9,
    10,
    11,  //J
    13,  //Q  — salta o cavaleiro (posição 12)
    14   //K  
};
 
//valores do offeset tiveram de ficar acima pra evitar o limite de 15 statements !!!
wchar_t carta_unicode(Carta c) {
    return 0x1F0A0 + 16 * c.naipe + offset_valor[c.valor];
}
