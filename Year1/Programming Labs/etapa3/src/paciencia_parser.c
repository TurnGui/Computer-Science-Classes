/**
 * @file paciencia_parser.c
 * @brief Implementação do parser de ficheiros DSL de paciências.
 */

#include "paciencia_parser.h"
#include <stdio.h>
#include <string.h>

/** @brief Tamanho máximo de uma linha do ficheiro DSL. */
#define MAX_LINHA 256

/* ── funções auxiliares de parsing ──────────────────────────── */

/**
 * @brief Remove tudo a partir do primeiro '#' numa linha.
 * @param linha String a modificar in-place.
 */
static void remover_comentario(char *linha) {
    char *pos = strchr(linha, '#');
    if (pos) *pos = '\0';
}

/**
 * @brief Converte uma string de flags da DSL num bitmask inteiro.
 */
/** @brief Tabela de conversão char→flag para flags de MOV/AUTO. */
typedef struct { char c; int flag; } EntradaFlag;

static const EntradaFlag FLAG_TABLE[] = {
    {'*', MOV_FLAG_SEMPRE},        {'+', MOV_FLAG_SEQUENCIA},
    {'[', MOV_FLAG_DECRESC},        {']', MOV_FLAG_CRESC},
    {'<', MOV_FLAG_INFERIOR},       {'>', MOV_FLAG_SUPERIOR},
    {'~', MOV_FLAG_ADJACENTE},      {'m', MOV_FLAG_MESMO_NAIPE},
    {'M', MOV_FLAG_MESMO_NAIPE_DEST},{'x', MOV_FLAG_NAIPE_ALT},
    {'X', MOV_FLAG_NAIPE_DIFER_DEST},{'c', MOV_FLAG_MESMA_COR},
    {'C', MOV_FLAG_MESMA_COR_DEST}, {'d', MOV_FLAG_COR_ALT},
    {'D', MOV_FLAG_COR_DIFER_DEST}, {'V', MOV_FLAG_DEST_VAZIA},
    {'a', MOV_FLAG_TOPO_AS},        {'A', MOV_FLAG_FUNDO_AS},
    {'k', MOV_FLAG_TOPO_REI},       {'K', MOV_FLAG_FUNDO_REI},
    {'\0', 0}
};

/**
 * @brief Procura o flag correspondente a um caractere na tabela.
 */
static int flag_para_char(char c) {
    for (int i = 0; FLAG_TABLE[i].c != '\0'; i++)
        if (FLAG_TABLE[i].c == c) return FLAG_TABLE[i].flag;
    return 0;
}

/**
 * @brief Converte uma string de flags da DSL num bitmask inteiro.
 */
int paciencia_flags_para_bitmask(const char *s) {
    int flags = 0;
    for (int i = 0; s[i] != '\0'; i++)
        flags |= flag_para_char(s[i]);
    return flags;
}

/**
 * @brief Procura o índice de um tipo de pilha pelo nome.
 */
int paciencia_indice_tipo(const Paciencia *p, const char *nome) {
    for (int i = 0; i < p->num_tipos; i++) {
        if (strcmp(p->tipos[i].nome, nome) == 0) return i;
    }
    return -1;
}

/* ── handlers de cada comando ────────────────────────────────── */

/**
 * @brief Processa o comando JOGO — guarda o nome da paciência.
 */
static void cmd_jogo(Paciencia *p, const char *linha) {
    sscanf(linha, "JOGO %31s", p->nome);
}

/**
 * @brief Processa o comando BARALHOS — guarda o número de baralhos.
 */
static void cmd_baralhos(Paciencia *p, const char *linha) {
    sscanf(linha, "BARALHOS %d", &p->num_baralhos);
}

/**
 * @brief Converte flags de tipo (=, _, ^, 1) para bitmask de TIPO_FLAG_*.
 */
static int tipo_flags_para_bitmask(const char *s) {
    int flags = 0;
    for (int i = 0; s[i] != '\0'; i++) {
        switch (s[i]) {
            case '=': flags |= TIPO_FLAG_TODAS_VISIVEIS;  break;
            case '_': flags |= TIPO_FLAG_NENHUMA_VISIVEL; break;
            case '^': flags |= TIPO_FLAG_SO_TOPO;         break;
            case '1': flags |= TIPO_FLAG_MAX_UMA;         break;
            default:  break;
        }
    }
    return flags;
}

/**
 * @brief Processa o comando TIPO — regista um novo tipo de pilha.
 */
static void cmd_tipo(Paciencia *p, const char *linha) {
    char nome[PAC_MAX_NOME], flags_str[PAC_MAX_NOME];
    if (sscanf(linha, "TIPO %31s %31s", nome, flags_str) != 2) return;
    if (p->num_tipos >= PAC_MAX_TIPOS) return;
    TipoPilha *t = &p->tipos[p->num_tipos];
    strncpy(t->nome, nome, PAC_MAX_NOME - 1);
    t->flags = tipo_flags_para_bitmask(flags_str);
    p->num_tipos++;
}

/**
 * @brief Processa o comando INIT — cria uma instância de pilha.
 */
static void cmd_init(Paciencia *p, const char *linha) {
    char nome[PAC_MAX_NOME];
    int n;
    if (sscanf(linha, "INIT %31s %d", nome, &n) != 2) return;
    if (p->num_pilhas >= PAC_MAX_PILHAS) return;
    int idx = paciencia_indice_tipo(p, nome);
    if (idx < 0) return;
    p->pilhas[p->num_pilhas].idx_tipo    = idx;
    p->pilhas[p->num_pilhas].cartas_init = n;
    p->num_pilhas++;
}

/**
 * @brief Processa os comandos MOV e AUTO — regista uma regra de movimento.
 */
static void cmd_mov(Paciencia *p, const char *linha, int automatico) {
    char cmd[16], orig[PAC_MAX_NOME], dest[PAC_MAX_NOME], flags_str[PAC_MAX_NOME];
    if (sscanf(linha, "%15s %31s %31s %31s", cmd, orig, dest, flags_str) != 4) return;
    if (p->num_regras >= PAC_MAX_REGRAS) return;
    int io = paciencia_indice_tipo(p, orig);
    int id = paciencia_indice_tipo(p, dest);
    if (io < 0 || id < 0) return;
    RegraMovimento *r = &p->regras[p->num_regras];
    r->idx_origem  = io;
    r->idx_destino = id;
    r->flags       = paciencia_flags_para_bitmask(flags_str);
    r->automatico  = automatico;
    p->num_regras++;
}

/**
 * @brief Processa o comando WIN — regista uma condição de vitória.
 */
static void cmd_win(Paciencia *p, const char *linha) {
    char nome[PAC_MAX_NOME];
    int n;
    if (sscanf(linha, "WIN %31s %d", nome, &n) != 2) return;
    if (p->num_wins >= PAC_MAX_WINS) return;
    int idx = paciencia_indice_tipo(p, nome);
    if (idx < 0) return;
    p->wins[p->num_wins].idx_tipo   = idx;
    p->wins[p->num_wins].num_cartas = n;
    p->num_wins++;
}

/* ── dispatcher de comandos ──────────────────────────────────── */

/**
 * @brief Identifica e despacha o comando de uma linha para o handler certo.
 */
static void processar_linha(Paciencia *p, char *linha) {
    remover_comentario(linha);
    char cmd[16] = {0};
    if (sscanf(linha, "%15s", cmd) != 1) return;
    if (strcmp(cmd, "JOGO")     == 0) cmd_jogo(p, linha);
    else if (strcmp(cmd, "BARALHOS") == 0) cmd_baralhos(p, linha);
    else if (strcmp(cmd, "TIPO")     == 0) cmd_tipo(p, linha);
    else if (strcmp(cmd, "INIT")     == 0) cmd_init(p, linha);
    else if (strcmp(cmd, "MOV")      == 0) cmd_mov(p, linha, 0);
    else if (strcmp(cmd, "AUTO")     == 0) cmd_mov(p, linha, 1);
    else if (strcmp(cmd, "WIN")      == 0) cmd_win(p, linha);
}

/* ── função pública ──────────────────────────────────────────── */

/**
 * @brief Lê um ficheiro DSL e preenche a struct Paciencia.
 */
int paciencia_ler(const char *caminho, Paciencia *p) {
    FILE *f = fopen(caminho, "r");
    if (!f) return 0;
    memset(p, 0, sizeof(Paciencia));
    char linha[MAX_LINHA];
    while (fgets(linha, sizeof(linha), f)) {
        processar_linha(p, linha);
    }
    fclose(f);
    return 1;
}
