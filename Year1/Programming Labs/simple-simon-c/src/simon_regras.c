/**
 * @file simon_regras.c
 * @brief Implementação das regras de jogabilidade do Simple Simon.
 */

#include "simon_regras.h"

/**
 * @brief Verifica se duas cartas são do mesmo naipe.
 */
int simon_mesmo_naipe(Carta a, Carta b) {
    return a.naipe == b.naipe;
}

/**
 * @brief Verifica se 'baixo' tem valor imediatamente inferior a 'cima'.
 */
int simon_valor_inferior(Carta cima, Carta baixo) {
    return baixo.valor == cima.valor - 1;
}

/**
 * @brief Verifica se uma sequência de n cartas do topo é válida.
 *
 * Percorre as n cartas do topo para baixo verificando
 * mesmo naipe e ordem decrescente consecutiva.
 */
int simon_sequencia_valida(const Pilha *p, int n) {
    int topo = p->topo;
    if (n <= 0 || n > topo + 1) return 0;
    for (int i = topo; i > topo - n ; i--) {
        Carta acima = p->cartas[i];
        Carta abaixo = p->cartas[i - 1];
        if (!simon_mesmo_naipe(acima, abaixo)) return 0;
        if (!simon_valor_inferior(abaixo, acima)) return 0;
    }
    return 1;
}

/**
 * @brief decide se é possível mover n cartas de uma pilha de origem para uma de destino. 
 * Primeiro verifica se a origem não está vazia e se n é válido. Depois há dois casos: 
 * se o destino estiver vazio devolve 1 diretamente (qualquer carta ou sequência pode ir para coluna vazia).
 *  Se o destino não estiver vazio, vai buscar a carta que ficaria no topo da sequência a mover (a mais funda das n) 
 * e verifica se tem valor imediatamente inferior ao topo do destino. Se n > 1 verifica também se a sequência é válida com simon_sequencia_valida.
 */
int simon_pode_mover(const Pilha *origem, const Pilha *destino, int n) {
    if (pilha_vazia(origem)) return 0;
    if (n <= 0 || n > pilha_tamanho(origem)) return 0;
    if (pilha_vazia(destino)) return 1;
    Carta topo_orig = origem->cartas[origem->topo - n + 1];
    Carta topo_dest = pilha_topo(destino);
    if (n == 1) return simon_valor_inferior(topo_dest, topo_orig);
    if (!simon_sequencia_valida(origem, n)) return 0;
    return simon_valor_inferior(topo_dest, topo_orig);
}

/**
 * @brief verifica se a pilha tem uma sequência K..A do mesmo naipe no topo, ou seja, 13 cartas prontas a ir para a fundação. 
 * Primeiro verifica se a pilha tem pelo menos 13 cartas, depois confirma que a carta na base da sequência (posição topo - 12) é um Rei (valor 13),
 *  e por fim chama simon_sequencia_valida com 13 para confirmar que toda a sequência é do mesmo naipe e em ordem decrescente.
 */
int simon_sequencia_completa(const Pilha *p) {
    if (pilha_tamanho(p) < 13) return 0;
    int base = p->topo - 12;
    if (p->cartas[base].valor != 13) return 0;
    return simon_sequencia_valida(p, 13);
}
