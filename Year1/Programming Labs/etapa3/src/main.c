/**
 * @file main.c
 * @brief Interface de linha de comandos da Etapa 3.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>
#include <time.h>
#include <dirent.h>

#include "carta.h"
#include "paciencia.h"
#include "paciencia_parser.h"
#include "jogo_estado.h"
#include "jogo_saveload.h"

#define PASTA_PACIENCIAS "paciencias"
#define MAX_PACIENCIAS   16
#define EXT_PACIENCIA    ".paciencia"

/* ── listagem ────────────────────────────────────────────────── */

/**
 * @brief Verifica se o ficheiro tem extensão .paciencia.
 */
static int tem_extensao(const char *nome) {
    return strstr(nome, EXT_PACIENCIA) != NULL;
}

/**
 * @brief Lista os ficheiros .paciencia da pasta.
 */
static int listar_paciencias(char nomes[][64], int max) {
    DIR *d = opendir(PASTA_PACIENCIAS);
    if (!d) return 0;
    int n = 0;
    struct dirent *entry;
    while (n < max && (entry = readdir(d)))
        if (tem_extensao(entry->d_name))
            strncpy(nomes[n++], entry->d_name, 63);
    closedir(d);
    return n;
}

/* ── menu ────────────────────────────────────────────────────── */

/**
 * @brief Mostra menu e devolve índice escolhido.
 */
static int menu_escolher(char nomes[][64], int n) {
    printf("Paciências disponíveis:\n");
    for (int i = 0; i < n; i++)
        printf("  %d) %s\n", i, nomes[i]);
    printf("Escolha: ");
    fflush(stdout);
    int escolha = -1;
    scanf("%d", &escolha);
    getchar();
    if (escolha < 0 || escolha >= n) return -1;
    return escolha;
}

/* ── mostrar estado ──────────────────────────────────────────── */

/**
 * @brief Converte carta para string legível (ex: "AS", "10H").
 */
static void carta_para_str(Carta c, char *buf) {
    static const char *valores[] = {
        "","A","2","3","4","5","6","7","8","9","10","J","Q","K"
    };
    static const char naipes[] = "CDHS";
    sprintf(buf, "%s%c", valores[c.valor], naipes[c.naipe]);
}

/**
 * @brief Mostra uma pilha numerada.
 */
static void mostrar_pilha(int i, const Pilha *p) {
    printf("[%2d] ", i);
    if (pilha_vazia(p)) { printf("(vazia)"); return; }
    char buf[8];
    for (int j = 0; j <= p->topo; j++) {
        carta_para_str(p->cartas[j], buf);
        printf("%s ", buf);
    }
}

/**
 * @brief Mostra todas as pilhas numeradas.
 */
static void mostrar_estado(const JogoEstado *e) {
    printf("\n");
    for (int i = 0; i < e->num_pilhas; i++) {
        mostrar_pilha(i, &e->pilhas[i]);
        printf("\n");
    }
    printf("\nComandos: m <orig> <dest> <n>  u (undo)  s <fich> (save)  l <fich> (load)  q (sair)\n");
    fflush(stdout);
}

/* ── comandos ────────────────────────────────────────────────── */

/**
 * @brief Comando mover.
 */
static int cmd_mover(JogoEstado *e, JogoHistorico *h, int orig, int dest, int n) {
    jogo_historico_guardar(h, e);
    if (!jogo_mover(e, orig, dest, n)) {
        jogo_historico_restaurar(h, e);
        printf("Jogada inválida.\n");
        return 1;
    }
    jogo_auto(e);
    return 1;
}

/**
 * @brief Comando undo.
 */
static int cmd_undo(JogoEstado *e, JogoHistorico *h) {
    if (!jogo_historico_restaurar(h, e))
        printf("Nada para desfazer.\n");
    return 1;
}

/**
 * @brief Comando gravar.
 */
static int cmd_gravar(const JogoEstado *e, const char *pac_nome, const char *fich) {
    if (!jogo_gravar(fich, e, pac_nome)) printf("Erro ao gravar.\n");
    else printf("Jogo gravado em %s.\n", fich);
    return 1;
}

/**
 * @brief Comando carregar.
 */
static int cmd_carregar(JogoEstado *e, JogoHistorico *h, const char *fich) {
    jogo_historico_guardar(h, e);
    if (!jogo_carregar(fich, e)) {
        jogo_historico_restaurar(h, e);
        printf("Erro ao carregar.\n");
    } else {
        printf("Jogo carregado de %s.\n", fich);
    }
    return 1;
}

/* ── loop ────────────────────────────────────────────────────── */

/**
 * @brief Lê e processa um comando.
 */
/**
 * @brief Tenta interpretar linha como comando de ficheiro (s/l).
 * @return 1 se consumiu, 0 se não reconheceu.
 */
static int tentar_cmd_ficheiro(JogoEstado *e, JogoHistorico *h,
                                const char *pac_nome, const char *linha) {
    char fich[64];
    if (sscanf(linha, " s %63s", fich) == 1)
        return cmd_gravar(e, pac_nome, fich);
    if (sscanf(linha, " l %63s", fich) == 1)
        return cmd_carregar(e, h, fich);
    return -1;
}

/**
 * @brief Lê e processa um comando do utilizador.
 */
/**
 * @brief Despacha comando de letra simples (u, q).
 * @return resultado do comando, ou -1 se não reconheceu.
 */
static int despachar_letra(JogoEstado *e, JogoHistorico *h, const char *linha) {
    char cmd;
    if (sscanf(linha, " %c", &cmd) != 1) return 1;
    if (cmd == 'u') return cmd_undo(e, h);
    if (cmd == 'q') return 0;
    printf("Comando inválido.\n");
    return 1;
}

/**
 * @brief Lê e processa um comando do utilizador.
 */
static int processar_comando(JogoEstado *e, JogoHistorico *h, const char *pac_nome) {
    char linha[128];
    printf("> ");
    fflush(stdout);
    if (!fgets(linha, sizeof(linha), stdin)) return 0;
    int orig, dest, n, r;
    if (sscanf(linha, " m %d %d %d", &orig, &dest, &n) == 3)
        return cmd_mover(e, h, orig, dest, n);
    r = tentar_cmd_ficheiro(e, h, pac_nome, linha);
    if (r >= 0) return r;
    return despachar_letra(e, h, linha);
}

/**
 * @brief Loop principal do jogo.
 */
static void loop_jogo(JogoEstado *e, JogoHistorico *h, const char *pac_nome) {
    jogo_auto(e);
    mostrar_estado(e);
    while (!jogo_ganhou(e)) {
        if (!processar_comando(e, h, pac_nome)) return;
        mostrar_estado(e);
    }
    if (jogo_ganhou(e)) printf("\nParabéns, ganhou! :)\n");
}

/* ── main ────────────────────────────────────────────────────── */

/**
 * @brief Carrega a paciência escolhida e inicializa o estado.
 * @return 1 se bem-sucedido, 0 em caso de erro.
 */
static int inicializar_jogo(JogoEstado *e, Paciencia *pac, const char *nome) {
    char caminho[128];
    snprintf(caminho, sizeof(caminho), "%s/%s", PASTA_PACIENCIAS, nome);
    if (!paciencia_ler(caminho, pac)) { printf("Erro ao ler a paciência.\n"); return 0; }
    jogo_estado_inicializar(e, pac, (unsigned)time(NULL));
    return 1;
}

/**
 * @brief Ponto de entrada.
 */
/**
 * @brief Mostra menu e devolve o caminho e nome escolhidos.
 * @return 1 se escolha válida, 0 caso contrário.
 */
static int escolher_paciencia(char nomes[][64], int *escolha) {
    int n = listar_paciencias(nomes, MAX_PACIENCIAS);
    if (n == 0) { printf("Nenhuma paciência encontrada em '%s'.\n", PASTA_PACIENCIAS); return 0; }
    *escolha = menu_escolher(nomes, n);
    if (*escolha < 0) { printf("Escolha inválida.\n"); return 0; }
    return 1;
}

/**
 * @brief Ponto de entrada.
 */
int main(void) {
    setlocale(LC_ALL, "");
    char nomes[MAX_PACIENCIAS][64];
    int escolha;
    if (!escolher_paciencia(nomes, &escolha)) return 1;
    Paciencia pac;
    JogoEstado e;
    if (!inicializar_jogo(&e, &pac, nomes[escolha])) return 1;
    JogoHistorico h;
    jogo_historico_inicializar(&h);
    loop_jogo(&e, &h, nomes[escolha]);
    return 0;
}
