#include "carta.h"

Carta carta_criar(int naipe, int valor) {
    Carta c;
    c.naipe = naipe;
    c.valor = valor;
    return c;
}

int carta_valida(Carta c) {
    return (c.naipe >= 0 && c.naipe < 4 && c.valor >= 1 && c.valor <= 13); // so temos 3 naipes e cada naipe tem 13 cartas
}

wchar_t carta_unicode(Carta c) {
    return 0x1F0A0 + c.valor + 16 * c.naipe; /* 0x1F0A0 pq é a posição onde começam os emojis das cartas em Unicode
                                                + c.valor pra andar para a carta dentro do naipe
                                                +16 * c.naipe pq cada naipe está separado em blocos de 16 codigos (copiado do exemplo do stor) */ 
}