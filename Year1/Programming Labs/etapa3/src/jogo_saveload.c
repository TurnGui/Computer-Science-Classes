/**
 * @file jogo_saveload.c
 * @brief Implementação de gravação e carregamento de estados de jogo.
 */

#include "jogo_saveload.h"
#include <stdio.h>
#include <string.h>

#define MAX_LINHA 512

/* ── conversão carta ↔ string ────────────────────────────────── */

/**
 * @brief Converte o valor de uma carta para string.
 */
static const char *valor_para_str(int v) {
    static const char *nomes[] = {
        "","A","2","3","4","5","6","7","8","9","10","J","Q","K"
    };
    return nomes[v];
}

/**
 * @brief Converte o naipe de uma carta para char.
 */
static char naipe_para_char(int n) {
    static const char naipes[] = "CDHS";
    return naipes[n];
}

/**
 * @brief Converte uma string "<valor><naipe>" para Carta.
 */
/**
 * @brief Extrai valor de carta de letra (A,J,Q,K).
 */
static int letra_para_valor(char c) {
    if (c == 'A') return 1;
    if (c == 'J') return 11;
    if (c == 'Q') return 12;
    if (c == 'K') return 13;
    return 0;
}

/**
 * @brief Extrai valor e char do naipe de uma string de carta.
 */
static void str_para_valor_naipe(const char *s, int *valor, char *naipe_c) {
    *valor = letra_para_valor(s[0]);
    if (*valor > 0)          { *naipe_c = s[1]; return; }
    if (s[0] == '1' && s[1] == '0') { *valor = 10; *naipe_c = s[2]; return; }
    *valor = s[0] - '0'; *naipe_c = s[1];
}

/**
 * @brief Converte uma string "<valor><naipe>" para Carta.
 */
/**
 * @brief Converte char de naipe para índice (0-3), -1 se inválido.
 */
static int char_para_naipe(char naipe_c) {
    static const char naipes[] = "CDHS";
    for (int i = 0; i < 4; i++)
        if (naipes[i] == naipe_c) return i;
    return -1;
}

/**
 * @brief Converte uma string "<valor><naipe>" para Carta.
 */
static int str_para_carta(const char *s, Carta *c) {
    int valor = 0;
    char naipe_c = '\0';
    str_para_valor_naipe(s, &valor, &naipe_c);
    if (valor < 1 || valor > 13) return 0;
    int naipe = char_para_naipe(naipe_c);
    if (naipe < 0) return 0;
    *c = carta_criar(naipe, valor);
    return 1;
}

/* ── gravar ──────────────────────────────────────────────────── */

/**
 * @brief Grava uma pilha numa linha (fundo→topo).
 */
static void gravar_pilha(FILE *f, const Pilha *p) {
    for (int i = 0; i <= p->topo; i++) {
        if (i > 0) fprintf(f, " ");
        fprintf(f, "%s%c",
            valor_para_str(p->cartas[i].valor),
            naipe_para_char(p->cartas[i].naipe));
    }
    fprintf(f, "\n");
}

/**
 * @brief Grava o estado actual num ficheiro.
 */
int jogo_gravar(const char *caminho, const JogoEstado *e, const char *pac_nome) {
    FILE *f = fopen(caminho, "w");
    if (!f) return 0;
    fprintf(f, "%s\n", pac_nome);
    for (int i = 0; i < e->num_pilhas; i++)
        gravar_pilha(f, &e->pilhas[i]);
    fclose(f);
    return 1;
}

/* ── carregar ────────────────────────────────────────────────── */

/**
 * @brief Carrega as cartas de uma linha para uma pilha usando strtok.
 */
static void carregar_pilha(Pilha *p, char *linha) {
    pilha_inicializar(p);
    char *token = strtok(linha, " \t\n\r");
    while (token != NULL) {
        Carta c;
        if (str_para_carta(token, &c)) pilha_push(p, c);
        token = strtok(NULL, " \t\n\r");
    }
}

/**
 * @brief Carrega um estado de jogo a partir de um ficheiro.
 */
int jogo_carregar(const char *caminho, JogoEstado *e) {
    FILE *f = fopen(caminho, "r");
    if (!f) return 0;
    char linha[MAX_LINHA];
    fgets(linha, sizeof(linha), f); /* primeira linha: nome da paciência */
    int i = 0;
    while (i < e->num_pilhas && fgets(linha, sizeof(linha), f))
        carregar_pilha(&e->pilhas[i++], linha);
    fclose(f);
    return 1;
}
