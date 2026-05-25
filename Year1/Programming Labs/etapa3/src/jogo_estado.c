/**
 * @file jogo_estado.c
 * @brief Implementação do estado genérico de uma partida.
 */

#include "jogo_estado.h"
#include <stdlib.h>
#include <string.h>

/* ── inicialização ───────────────────────────────────────────── */

/**
 * @brief Cria um stock com num_baralhos baralhos embaralhados.
 */
static void criar_stock(Pilha *stock, int num_baralhos) {
    pilha_inicializar(stock);
    for (int b = 0; b < num_baralhos; b++) {
        Baralho bar;
        baralho_inicializar(&bar);
        baralho_embaralhar(&bar);
        while (!baralho_vazio(&bar))
            pilha_push(stock, baralho_retirar(&bar));
    }
}

/**
 * @brief Distribui as cartas do stock pelas pilhas segundo os INIT.
 */
static void distribuir(JogoEstado *e, Pilha *stock) {
    for (int i = 0; i < e->num_pilhas; i++) {
        pilha_inicializar(&e->pilhas[i]);
        int n = e->pac->pilhas[i].cartas_init;
        for (int j = 0; j < n && !pilha_vazia(stock); j++)
            pilha_push(&e->pilhas[i], pilha_pop(stock));
    }
}

/**
 * @brief Inicializa o estado do jogo a partir de uma paciência e uma seed.
 */
void jogo_estado_inicializar(JogoEstado *e, const Paciencia *pac, unsigned seed) {
    srand(seed);
    e->pac        = pac;
    e->num_pilhas = pac->num_pilhas;
    Pilha stock;
    int nb = pac->num_baralhos > 0 ? pac->num_baralhos : 1;
    criar_stock(&stock, nb);
    distribuir(e, &stock);
}

/* ── vitória ─────────────────────────────────────────────────── */

/**
 * @brief Verifica se todas as pilhas de um tipo têm exactamente n cartas.
 */
static int win_tipo_ok(const JogoEstado *e, int idx_tipo, int n) {
    for (int i = 0; i < e->num_pilhas; i++) {
        if (e->pac->pilhas[i].idx_tipo != idx_tipo) continue;
        if (pilha_tamanho(&e->pilhas[i]) != n) return 0;
    }
    return 1;
}

/**
 * @brief Verifica se todas as condições WIN estão satisfeitas.
 */
int jogo_ganhou(const JogoEstado *e) {
    for (int i = 0; i < e->pac->num_wins; i++) {
        int idx = e->pac->wins[i].idx_tipo;
        int n   = e->pac->wins[i].num_cartas;
        if (!win_tipo_ok(e, idx, n)) return 0;
    }
    return 1;
}

/* ── validação de movimentos ─────────────────────────────────── */

/**
 * @brief Verifica se a cor de uma carta é vermelha (Ouros=1 ou Copas=2).
 */
static int carta_vermelha(Carta c) {
    return c.naipe == 1 || c.naipe == 2;
}

/**
 * @brief Verifica flags de relação entre topo da sequência e topo do destino.
 */
/**
 * @brief Verifica flags de valor entre topo da sequência e topo do destino.
 */
static int check_destino_valor(int dif, int flags) {
    if ((flags & MOV_FLAG_INFERIOR)  && dif != -1) return 0;
    if ((flags & MOV_FLAG_SUPERIOR)  && dif !=  1) return 0;
    if ((flags & MOV_FLAG_ADJACENTE) && dif != -1 && dif != 1) return 0;
    return 1;
}

/**
 * @brief Verifica flags de naipe/cor entre topo da sequência e topo do destino.
 */
static int check_destino_naipe(Carta a, Carta b, int flags) {
    if ((flags & MOV_FLAG_MESMO_NAIPE_DEST) && a.naipe != b.naipe) return 0;
    if ((flags & MOV_FLAG_NAIPE_DIFER_DEST) && a.naipe == b.naipe) return 0;
    if ((flags & MOV_FLAG_MESMA_COR_DEST)   && carta_vermelha(a) != carta_vermelha(b)) return 0;
    if ((flags & MOV_FLAG_COR_DIFER_DEST)   && carta_vermelha(a) == carta_vermelha(b)) return 0;
    return 1;
}

/**
 * @brief Verifica flags de relação entre topo da sequência e topo do destino.
 */
static int check_flags_destino(Carta topo_orig, Carta topo_dest, int flags) {
    int dif = topo_orig.valor - topo_dest.valor;
    if (!check_destino_valor(dif, flags)) return 0;
    if (!check_destino_naipe(topo_orig, topo_dest, flags)) return 0;
    return 1;
}

/**
 * @brief Verifica flags de ordem entre duas cartas consecutivas da sequência.
 */
static int check_par_ordem(Carta acima, Carta abaixo, int flags) {
    int dif = acima.valor - abaixo.valor;
    if ((flags & MOV_FLAG_DECRESC) && dif != -1) return 0;
    if ((flags & MOV_FLAG_CRESC)   && dif != 1)  return 0;
    return 1;
}

/**
 * @brief Verifica flags de naipe/cor entre duas cartas consecutivas da sequência.
 */
static int check_par_naipe(Carta acima, Carta abaixo, int flags) {
    if ((flags & MOV_FLAG_MESMO_NAIPE) && acima.naipe != abaixo.naipe) return 0;
    if ((flags & MOV_FLAG_NAIPE_ALT)   && acima.naipe == abaixo.naipe) return 0;
    if ((flags & MOV_FLAG_MESMA_COR)   && carta_vermelha(acima) != carta_vermelha(abaixo)) return 0;
    if ((flags & MOV_FLAG_COR_ALT)     && carta_vermelha(acima) == carta_vermelha(abaixo)) return 0;
    return 1;
}

/**
 * @brief Verifica flags internas da sequência a mover.
 */
static int check_flags_sequencia(const Pilha *orig, int n, int flags) {
    if (n > 1 && !(flags & MOV_FLAG_SEQUENCIA)) return 0;
    int topo = orig->topo;
    for (int i = topo; i > topo - n + 1; i--) {
        Carta acima = orig->cartas[i];
        Carta abaixo = orig->cartas[i - 1];
        if (!check_par_ordem(acima, abaixo, flags)) return 0;
        if (!check_par_naipe(acima, abaixo, flags)) return 0;
    }
    return 1;
}

/**
 * @brief Verifica flags de topo e fundo da sequência (Ás, Rei).
 */
static int check_flags_extremos(const Pilha *orig, int n, int flags) {
    Carta topo_seq  = orig->cartas[orig->topo];
    Carta fundo_seq = orig->cartas[orig->topo - n + 1];
    if ((flags & MOV_FLAG_TOPO_AS)   && topo_seq.valor  != 1)  return 0;
    if ((flags & MOV_FLAG_FUNDO_AS)  && fundo_seq.valor != 1)  return 0;
    if ((flags & MOV_FLAG_TOPO_REI)  && topo_seq.valor  != 13) return 0;
    if ((flags & MOV_FLAG_FUNDO_REI) && fundo_seq.valor != 13) return 0;
    return 1;
}

/**
 * @brief Verifica se uma regra específica permite o movimento.
 */
static int regra_permite(const RegraMovimento *r, const Pilha *orig,
                          const Pilha *dest, int n) {
    if (pilha_vazia(orig)) return 0;
    if (n <= 0 || n > pilha_tamanho(orig)) return 0;
    if (r->flags & MOV_FLAG_DEST_VAZIA) return pilha_vazia(dest);
    if (r->flags & MOV_FLAG_SEMPRE) return 1;
    if (!pilha_vazia(dest)) {
        Carta topo_orig = orig->cartas[orig->topo - n + 1];
        if (!check_flags_destino(topo_orig, pilha_topo(dest), r->flags)) return 0;
    }
    if (!check_flags_sequencia(orig, n, r->flags)) return 0;
    if (!check_flags_extremos(orig, n, r->flags)) return 0;
    return 1;
}

/**
 * @brief Verifica se os tipos das pilhas correspondem aos da regra.
 */
static int tipos_correspondem(const JogoEstado *e, int orig, int dest,
                               const RegraMovimento *r) {
    int to = e->pac->pilhas[orig].idx_tipo;
    int td = e->pac->pilhas[dest].idx_tipo;
    return to == r->idx_origem && td == r->idx_destino;
}

/**
 * @brief Verifica se alguma regra MOV permite o movimento.
 */
int jogo_pode_mover(const JogoEstado *e, int orig, int dest, int n) {
    if (orig < 0 || orig >= e->num_pilhas) return 0;
    if (dest < 0 || dest >= e->num_pilhas) return 0;
    if (orig == dest) return 0;
    for (int i = 0; i < e->pac->num_regras; i++) {
        const RegraMovimento *r = &e->pac->regras[i];
        if (r->automatico) continue;
        if (!tipos_correspondem(e, orig, dest, r)) continue;
        if (regra_permite(r, &e->pilhas[orig], &e->pilhas[dest], n)) return 1;
    }
    return 0;
}

/* ── movimento ───────────────────────────────────────────────── */

/**
 * @brief Move n cartas do topo de origem para destino.
 */
static void mover_cartas(Pilha *origem, Pilha *destino, int n) {
    Carta tmp[MAX_CARTAS_PILHA];
    for (int i = 0; i < n; i++) tmp[i] = pilha_pop(origem);
    for (int i = n - 1; i >= 0; i--) pilha_push(destino, tmp[i]);
}

/**
 * @brief Executa o movimento se for válido.
 */
int jogo_mover(JogoEstado *e, int orig, int dest, int n) {
    if (!jogo_pode_mover(e, orig, dest, n)) return 0;
    mover_cartas(&e->pilhas[orig], &e->pilhas[dest], n);
    return 1;
}

/* ── automáticos ─────────────────────────────────────────────── */

/**
 * @brief Tenta aplicar uma regra AUTO a um par de pilhas.
 */
static int auto_par(JogoEstado *e, int orig, int dest, const RegraMovimento *r) {
    if (!tipos_correspondem(e, orig, dest, r)) return 0;
    int n = pilha_tamanho(&e->pilhas[orig]);
    for (; n >= 1; n--) {
        if (regra_permite(r, &e->pilhas[orig], &e->pilhas[dest], n)) {
            mover_cartas(&e->pilhas[orig], &e->pilhas[dest], n);
            return 1;
        }
    }
    return 0;
}

/**
 * @brief Executa todos os movimentos automáticos possíveis.
 */
int jogo_auto(JogoEstado *e) {
    int total = 0, encontrou = 1;
    while (encontrou) {
        encontrou = 0;
        for (int i = 0; i < e->pac->num_regras; i++) {
            const RegraMovimento *r = &e->pac->regras[i];
            if (!r->automatico) continue;
            for (int o = 0; o < e->num_pilhas; o++)
                for (int d = 0; d < e->num_pilhas; d++)
                    if (o != d && auto_par(e, o, d, r)) { encontrou = 1; total++; }
        }
    }
    return total;
}

/* ── tem jogada ──────────────────────────────────────────────── */

/**
 * @brief Verifica se existe alguma jogada MOV válida.
 */
int jogo_tem_jogada(const JogoEstado *e) {
    for (int o = 0; o < e->num_pilhas; o++)
        for (int d = 0; d < e->num_pilhas; d++)
            if (o != d && jogo_pode_mover(e, o, d, 1)) return 1;
    return 0;
}

/* ── undo ────────────────────────────────────────────────────── */

/**
 * @brief Inicializa o histórico de undo vazio.
 */
void jogo_historico_inicializar(JogoHistorico *h) {
    h->topo = -1;
}

/**
 * @brief Guarda o estado actual no histórico.
 */
int jogo_historico_guardar(JogoHistorico *h, const JogoEstado *e) {
    if (h->topo >= MAX_UNDO - 1) return 0;
    h->topo++;
    memcpy(h->snapshots[h->topo].pilhas, e->pilhas, sizeof(e->pilhas));
    h->snapshots[h->topo].num_pilhas = e->num_pilhas;
    return 1;
}

/**
 * @brief Restaura o último estado guardado no histórico.
 */
int jogo_historico_restaurar(JogoHistorico *h, JogoEstado *e) {
    if (h->topo < 0) return 0;
    memcpy(e->pilhas, h->snapshots[h->topo].pilhas, sizeof(e->pilhas));
    e->num_pilhas = h->snapshots[h->topo].num_pilhas;
    h->topo--;
    return 1;
}
