#ifndef CARTA_H
#define CARTA_H

#include <wchar.h> //para o simbolo das cartas

// Estrutura da carta
typedef struct {
    int naipe;   //0-3
    int valor;   // 1-13
} Carta;

// Cria uma carta
Carta carta_criar(int naipe, int valor);

// Verifica se a carta é válida ( 1 valida , 0 invalida)
int carta_valida(Carta c);

// Devolve o emoji da carta
wchar_t carta_unicode(Carta c);

#endif