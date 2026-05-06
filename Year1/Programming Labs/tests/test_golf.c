#include <CUnit/CUnit.h>
#include <CUnit/Basic.h>
#include "carta.h"
#include "baralho.h"
#include "golf.h"
#include "golf_estado.h"

/* ===== carta.c ===== */

void test_carta_criar(void) {
    Carta c = carta_criar(2, 7);
    CU_ASSERT_EQUAL(c.naipe, 2);
    CU_ASSERT_EQUAL(c.valor, 7);
}

void test_carta_valida_ok(void) {
    Carta c = carta_criar(0, 1);
    CU_ASSERT_TRUE(carta_valida(c));

    Carta c2 = carta_criar(3, 13);
    CU_ASSERT_TRUE(carta_valida(c2));
}

void test_carta_valida_invalida(void) {
    Carta c1 = carta_criar(-1, 5);
    CU_ASSERT_FALSE(carta_valida(c1));

    Carta c2 = carta_criar(4, 5);
    CU_ASSERT_FALSE(carta_valida(c2));

    Carta c3 = carta_criar(0, 0);
    CU_ASSERT_FALSE(carta_valida(c3));

    Carta c4 = carta_criar(0, 14);
    CU_ASSERT_FALSE(carta_valida(c4));
}

void test_carta_unicode(void) {
    /* Ás de espadas: naipe 0, valor 1 → 0x1F0A1 */
    Carta as_espadas = carta_criar(0, 1);
    CU_ASSERT_EQUAL(carta_unicode(as_espadas), 0x1F0A1);

    /* Rei de espadas: naipe 0, valor 13 → offset 14 → 0x1F0AE */
    Carta rei_espadas = carta_criar(0, 13);
    CU_ASSERT_EQUAL(carta_unicode(rei_espadas), 0x1F0AE);
}

/* ===== baralho.c ===== */

void test_baralho_inicializar(void) {
    Baralho b;
    baralho_inicializar(&b);
    CU_ASSERT_EQUAL(b.topo, 0);
    /* primeira carta: naipe 0 valor 1 */
    CU_ASSERT_EQUAL(b.cartas[0].naipe, 0);
    CU_ASSERT_EQUAL(b.cartas[0].valor, 1);
    /* última carta: naipe 3 valor 13 */
    CU_ASSERT_EQUAL(b.cartas[51].naipe, 3);
    CU_ASSERT_EQUAL(b.cartas[51].valor, 13);
}

void test_baralho_retirar(void) {
    Baralho b;
    baralho_inicializar(&b);
    Carta c = baralho_retirar(&b);
    CU_ASSERT_EQUAL(c.naipe, 0);
    CU_ASSERT_EQUAL(c.valor, 1);
    CU_ASSERT_EQUAL(b.topo, 1);
}

void test_baralho_vazio(void) {
    Baralho b;
    baralho_inicializar(&b);
    CU_ASSERT_FALSE(baralho_vazio(&b));
    /* esvaziar o baralho */
    for (int i = 0; i < TAMANHO_BARALHO; i++) baralho_retirar(&b);
    CU_ASSERT_TRUE(baralho_vazio(&b));
}

void test_baralho_embaralhar(void) {
    Baralho b;
    baralho_inicializar(&b);
    baralho_embaralhar(&b);
    CU_ASSERT_EQUAL(b.topo, 0);
    /* depois de embaralhar ainda tem 52 cartas válidas */
    int validas = 0;
    for (int i = 0; i < TAMANHO_BARALHO; i++) {
        if (carta_valida(b.cartas[i])) validas++;
    }
    CU_ASSERT_EQUAL(validas, TAMANHO_BARALHO);
}

/* ===== golf.c ===== */

void test_golf_carta_pode_jogar_valido(void) {
    Carta mesa    = carta_criar(0, 7);
    Carta acima   = carta_criar(1, 8);
    Carta abaixo  = carta_criar(2, 6);
    CU_ASSERT_TRUE(golf_carta_pode_jogar(mesa, acima));
    CU_ASSERT_TRUE(golf_carta_pode_jogar(mesa, abaixo));
}

void test_golf_carta_pode_jogar_invalido(void) {
    Carta mesa  = carta_criar(0, 7);
    Carta longe = carta_criar(1, 5);
    Carta igual = carta_criar(0, 7);
    CU_ASSERT_FALSE(golf_carta_pode_jogar(mesa, longe));
    CU_ASSERT_FALSE(golf_carta_pode_jogar(mesa, igual));
}

/* ===== golf_estado.c ===== */

void test_golf_estado_inicializar(void) {
    GolfEstado e;
    golf_estado_inicializar(&e, 42);
    /* depois de inicializar: stock não está vazio */
    CU_ASSERT_FALSE(golf_stock_vazio(&e));
    /* nenhuma coluna está vazia */
    for (int c = 0; c < GOLF_COLUNAS; c++) {
        CU_ASSERT_FALSE(golf_coluna_vazia(&e, c));
    }
    /* descarte é carta válida */
    CU_ASSERT_TRUE(carta_valida(golf_descarte(&e)));
}

void test_golf_coluna_vazia(void) {
    GolfEstado e;
    golf_estado_inicializar(&e, 1);
    /* forçar topo[0] = -1 para simular coluna vazia */
    e.topo[0] = -1;
    CU_ASSERT_TRUE(golf_coluna_vazia(&e, 0));
    CU_ASSERT_FALSE(golf_coluna_vazia(&e, 1));
}

void test_golf_topo_coluna(void) {
    GolfEstado e;
    golf_estado_inicializar(&e, 1);
    Carta topo = golf_topo_coluna(&e, 0);
    CU_ASSERT_TRUE(carta_valida(topo));
    /* deve coincidir com mesa[0][topo[0]] */
    CU_ASSERT_EQUAL(topo.naipe, e.mesa[0][e.topo[0]].naipe);
    CU_ASSERT_EQUAL(topo.valor, e.mesa[0][e.topo[0]].valor);
}

void test_golf_descarte(void) {
    GolfEstado e;
    golf_estado_inicializar(&e, 7);
    Carta d = golf_descarte(&e);
    CU_ASSERT_TRUE(carta_valida(d));
}

void test_golf_pode_jogar_coluna(void) {
    GolfEstado e;
    golf_estado_inicializar(&e, 1);
    /* coluna inválida */
    CU_ASSERT_FALSE(golf_pode_jogar_coluna(&e, -1));
    CU_ASSERT_FALSE(golf_pode_jogar_coluna(&e, GOLF_COLUNAS));
    /* coluna vazia */
    e.topo[0] = -1;
    CU_ASSERT_FALSE(golf_pode_jogar_coluna(&e, 0));
}

void test_golf_jogar_coluna_invalida(void) {
    GolfEstado e;
    golf_estado_inicializar(&e, 1);
    /* jogar coluna fora de intervalo deve falhar */
    int res = golf_jogar_coluna(&e, -1);
    CU_ASSERT_EQUAL(res, 0);
}

void test_golf_puxar_stock(void) {
    GolfEstado e;
    golf_estado_inicializar(&e, 1);
    Carta antes = golf_descarte(&e);
    int ok = golf_puxar_stock(&e);
    CU_ASSERT_TRUE(ok);
    /* descarte mudou (pode ter valor diferente) */
    Carta depois = golf_descarte(&e);
    CU_ASSERT_TRUE(carta_valida(depois));
    (void)antes; /* pode coincidir por acaso, só verificamos que é válida */
}

void test_golf_ganhou(void) {
    GolfEstado e;
    golf_estado_inicializar(&e, 1);
    CU_ASSERT_FALSE(golf_ganhou(&e));
    /* esvaziar todas as colunas */
    for (int c = 0; c < GOLF_COLUNAS; c++) e.topo[c] = -1;
    CU_ASSERT_TRUE(golf_ganhou(&e));
}

void test_golf_tem_jogada(void) {
    GolfEstado e;
    golf_estado_inicializar(&e, 1);
    /* forçar estado em que nenhuma coluna tem jogada válida:
       pôr descarte = valor 7 e todos os topos = valor 7 (diferença 0) */
    e.descarte = carta_criar(0, 7);
    for (int c = 0; c < GOLF_COLUNAS; c++) {
        e.topo[c] = 0;
        e.mesa[c][0] = carta_criar(c % 4, 7);
    }
    CU_ASSERT_FALSE(golf_tem_jogada(&e));

    /* agora pôr um topo adjacente */
    e.mesa[0][0] = carta_criar(0, 8);
    CU_ASSERT_TRUE(golf_tem_jogada(&e));
}

/* ===== main dos testes ===== */

int main(void) {
    CU_initialize_registry();

    CU_pSuite s_carta = CU_add_suite("carta", NULL, NULL);
    CU_add_test(s_carta, "carta_criar",          test_carta_criar);
    CU_add_test(s_carta, "carta_valida_ok",      test_carta_valida_ok);
    CU_add_test(s_carta, "carta_valida_invalida",test_carta_valida_invalida);
    CU_add_test(s_carta, "carta_unicode",        test_carta_unicode);

    CU_pSuite s_baralho = CU_add_suite("baralho", NULL, NULL);
    CU_add_test(s_baralho, "baralho_inicializar", test_baralho_inicializar);
    CU_add_test(s_baralho, "baralho_retirar",     test_baralho_retirar);
    CU_add_test(s_baralho, "baralho_vazio",       test_baralho_vazio);
    CU_add_test(s_baralho, "baralho_embaralhar",  test_baralho_embaralhar);

    CU_pSuite s_golf = CU_add_suite("golf", NULL, NULL);
    CU_add_test(s_golf, "pode_jogar_valido",   test_golf_carta_pode_jogar_valido);
    CU_add_test(s_golf, "pode_jogar_invalido", test_golf_carta_pode_jogar_invalido);

    CU_pSuite s_estado = CU_add_suite("golf_estado", NULL, NULL);
    CU_add_test(s_estado, "inicializar",        test_golf_estado_inicializar);
    CU_add_test(s_estado, "coluna_vazia",       test_golf_coluna_vazia);
    CU_add_test(s_estado, "topo_coluna",        test_golf_topo_coluna);
    CU_add_test(s_estado, "descarte",           test_golf_descarte);
    CU_add_test(s_estado, "pode_jogar_coluna",  test_golf_pode_jogar_coluna);
    CU_add_test(s_estado, "jogar_coluna_inv",   test_golf_jogar_coluna_invalida);
    CU_add_test(s_estado, "puxar_stock",        test_golf_puxar_stock);
    CU_add_test(s_estado, "ganhou",             test_golf_ganhou);
    CU_add_test(s_estado, "tem_jogada",         test_golf_tem_jogada);

    CU_basic_set_mode(CU_BRM_VERBOSE);
    CU_basic_run_tests();
    int falhas = (int)CU_get_number_of_failures();
    CU_cleanup_registry();
    return falhas;
}