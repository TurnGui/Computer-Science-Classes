#include <stdio.h>
#include <locale.h>
#include <time.h>
#include <wchar.h>

#include "carta.h"
#include "golf_estado.h"

static void imprimir_espaco_carta(void) {
    wprintf(L"   ");
}

static void imprimir_carta(Carta c) {
    wprintf(L"%lc ", carta_unicode(c));
}

static void ui_mostrar(const GolfEstado *e) {

    wprintf(L"\nDescarte: ");
    imprimir_carta(golf_descarte(e));

    wprintf(L"   Stock: %d\n\n", TAMANHO_BARALHO - e->stock.topo);

    for (int linha = 0; linha < GOLF_LINHAS; linha++) {
        wprintf(L"   ");
        for (int col = 0; col < GOLF_COLUNAS; col++) {
            if (linha <= e->topo[col]) {
                imprimir_carta(e->mesa[col][linha]);
            } else {
                imprimir_espaco_carta();
            }
        }
        wprintf(L"\n");
    }

    wprintf(L"\nComandos: j <0-6> (jogar), p (puxar), q (sair)\n");
}

static int ler_comando(char *cmd, int *arg) {
    char linha[64];

    if (!fgets(linha, sizeof(linha), stdin)) return 0;

    *cmd = '\0';
    *arg = -1;

    if (sscanf(linha, " %c %d", cmd, arg) >= 1) return 1;
    return 0;
}

static int jogo_terminou(const GolfEstado *e) {
    if (golf_ganhou(e)) return 1;
    if (golf_stock_vazio(e) && !golf_tem_jogada(e)) return 1;
    return 0;
}

static void mostrar_resultado(const GolfEstado *e) {
    if (golf_ganhou(e)) {
        wprintf(L"\n Parabéns ganhou! Nice :)\n");
    } else if (golf_stock_vazio(e) && !golf_tem_jogada(e)) {
        wprintf(L"\nSem baralho e sem jogadas possíveis. Perdeu :(\n");
    }
}

int main(void) {
    setlocale(LC_ALL, "");

    GolfEstado e;
    golf_estado_inicializar(&e, (unsigned)time(NULL));

    while (!jogo_terminou(&e)) {
        char cmd;
        int arg;

        ui_mostrar(&e);

        wprintf(L"> ");
        if (!ler_comando(&cmd, &arg)) break;

        if (cmd == 'q') break;

        if (cmd == 'p') {
            if (!golf_puxar_stock(&e)) {
                wprintf(L"Baralho vazio!\n");
            }
        } else if (cmd == 'j') {
            if (!golf_jogar_coluna(&e, arg)) {
                wprintf(L"Jogada inválida.\n");
            }
        } else {
            wprintf(L"Comando inválido.\n");
        }
    }

    mostrar_resultado(&e);
    return 0;
}