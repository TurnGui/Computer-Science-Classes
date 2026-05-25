#include <CUnit/CUnit.h>
#include <CUnit/Basic.h>
#include "carta.h"
#include "pilha.h"
#include "simon_regras.h"
#include "simon_estado.h"

/* ===== pilha ===== */

/* verifica que após inicializar a pilha está vazia e tem tamanho 0 */
void test_pilha_inicializar(void) {
    Pilha p;
    pilha_inicializar(&p);
    CU_ASSERT_TRUE(pilha_vazia(&p));
    CU_ASSERT_EQUAL(pilha_tamanho(&p), 0);
}

/* verifica que push adiciona a carta corretamente e pop a remove */
void test_pilha_push_pop(void) {
    Pilha p;
    pilha_inicializar(&p);
    Carta c = carta_criar(0, 7);
    CU_ASSERT_TRUE(pilha_push(&p, c));
    CU_ASSERT_EQUAL(pilha_tamanho(&p), 1);
    CU_ASSERT_FALSE(pilha_vazia(&p));
    Carta t = pilha_topo(&p);
    CU_ASSERT_EQUAL(t.valor, 7);
    Carta r = pilha_pop(&p);
    CU_ASSERT_EQUAL(r.valor, 7);
    CU_ASSERT_TRUE(pilha_vazia(&p));
}

/* verifica que pilha_carta devolve a carta certa pela posição — 0 é o fundo */
void test_pilha_carta(void) {
    Pilha p;
    pilha_inicializar(&p);
    pilha_push(&p, carta_criar(0, 5));
    pilha_push(&p, carta_criar(0, 4));
    CU_ASSERT_EQUAL(pilha_carta(&p, 0).valor, 5); /* fundo */
    CU_ASSERT_EQUAL(pilha_carta(&p, 1).valor, 4); /* topo  */
}

/* verifica que a pilha deteta quando está cheia e rejeita mais cartas */
void test_pilha_cheia(void) {
    Pilha p;
    pilha_inicializar(&p);
    CU_ASSERT_FALSE(pilha_cheia(&p));
    for (int i = 0; i < MAX_CARTAS_PILHA; i++)
        pilha_push(&p, carta_criar(i % 4, (i % 13) + 1));
    CU_ASSERT_TRUE(pilha_cheia(&p));
    CU_ASSERT_FALSE(pilha_push(&p, carta_criar(0, 1)));
}

/* ===== simon_regras ===== */

/* verifica que duas cartas do mesmo naipe passam e de naipes diferentes falham */
void test_mesmo_naipe(void) {
    Carta a = carta_criar(1, 5);
    Carta b = carta_criar(1, 9);
    Carta c = carta_criar(2, 5);
    CU_ASSERT_TRUE(simon_mesmo_naipe(a, b));
    CU_ASSERT_FALSE(simon_mesmo_naipe(a, c));
}

/* verifica que só passa quando a carta de baixo tem valor exatamente 1 abaixo */
void test_valor_inferior(void) {
    Carta cima   = carta_criar(0, 8);
    Carta baixo  = carta_criar(0, 7);
    Carta errada = carta_criar(0, 6);
    CU_ASSERT_TRUE(simon_valor_inferior(cima, baixo));
    CU_ASSERT_FALSE(simon_valor_inferior(cima, errada));
    CU_ASSERT_FALSE(simon_valor_inferior(cima, cima));
}

/* verifica sequência válida (mesmo naipe, decrescente) e inválida (naipe diferente) */
void test_sequencia_valida(void) {
    Pilha p;
    pilha_inicializar(&p);
    pilha_push(&p, carta_criar(0, 8));
    pilha_push(&p, carta_criar(0, 7));
    pilha_push(&p, carta_criar(0, 6));
    CU_ASSERT_TRUE(simon_sequencia_valida(&p, 3));
    CU_ASSERT_TRUE(simon_sequencia_valida(&p, 1));
    /* carta de naipe diferente quebra a sequência */
    pilha_push(&p, carta_criar(1, 5));
    CU_ASSERT_FALSE(simon_sequencia_valida(&p, 4));
    CU_ASSERT_TRUE(simon_sequencia_valida(&p, 1)); /* 1 carta é sempre válida */
}

/* verifica que qualquer carta pode mover para destino vazio */
void test_pode_mover_para_vazia(void) {
    Pilha orig, dest;
    pilha_inicializar(&orig);
    pilha_inicializar(&dest);
    pilha_push(&orig, carta_criar(0, 5));
    CU_ASSERT_TRUE(simon_pode_mover(&orig, &dest, 1));
}

/* verifica que só pode mover se o valor for imediatamente inferior ao destino */
void test_pode_mover_valor_inferior(void) {
    Pilha orig, dest;
    pilha_inicializar(&orig);
    pilha_inicializar(&dest);
    pilha_push(&dest, carta_criar(0, 8));
    pilha_push(&orig, carta_criar(1, 7)); /* valor inferior — pode */
    CU_ASSERT_TRUE(simon_pode_mover(&orig, &dest, 1));
    Pilha orig2;
    pilha_inicializar(&orig2);
    pilha_push(&orig2, carta_criar(1, 6)); /* diferença 2 — não pode */
    CU_ASSERT_FALSE(simon_pode_mover(&orig2, &dest, 1));
}

/* verifica que só deteta sequência completa quando tem K..A do mesmo naipe */
void test_sequencia_completa(void) {
    Pilha p;
    pilha_inicializar(&p);
    /* sequência incompleta — falta o Ás */
    for (int v = 13; v >= 2; v--) pilha_push(&p, carta_criar(0, v));
    CU_ASSERT_FALSE(simon_sequencia_completa(&p));
    /* completar com o Ás — agora é K..A do mesmo naipe */
    pilha_push(&p, carta_criar(0, 1));
    CU_ASSERT_TRUE(simon_sequencia_completa(&p));
}

/* ===== simon_estado ===== */

/* verifica que as 52 cartas ficam distribuídas pelas 10 colunas e as fundações vazias */
void test_simon_inicializar(void) {
    SimonEstado e;
    simon_estado_inicializar(&e, 42);
    int total = 0;
    for (int i = 0; i < SIMON_COLUNAS; i++)
        total += pilha_tamanho(&e.tab[i]);
    CU_ASSERT_EQUAL(total, 52);
    for (int f = 0; f < SIMON_FUNDACOES; f++)
        CU_ASSERT_TRUE(pilha_vazia(&e.fund[f]));
}

/* verifica que não ganha com colunas cheias e ganha quando todas ficam vazias */
void test_simon_ganhou(void) {
    SimonEstado e;
    simon_estado_inicializar(&e, 1);
    CU_ASSERT_FALSE(simon_ganhou(&e));
    for (int i = 0; i < SIMON_COLUNAS; i++) pilha_inicializar(&e.tab[i]);
    CU_ASSERT_TRUE(simon_ganhou(&e));
}

/* verifica que índices inválidos e origem igual a destino são rejeitados */
void test_simon_pode_jogar_invalido(void) {
    SimonEstado e;
    simon_estado_inicializar(&e, 1);
    CU_ASSERT_FALSE(simon_pode_jogar(&e, -1, 0, 1));
    CU_ASSERT_FALSE(simon_pode_jogar(&e, 0, SIMON_COLUNAS, 1));
    CU_ASSERT_FALSE(simon_pode_jogar(&e, 0, 0, 1)); /* origem == destino */
}

/* verifica que uma jogada válida move a carta e atualiza os tamanhos das colunas */
void test_simon_jogar(void) {
    SimonEstado e;
    simon_estado_inicializar(&e, 1);
    /* forçar situação conhecida: 8 pode ir para cima do 9 */
    pilha_inicializar(&e.tab[0]);
    pilha_inicializar(&e.tab[1]);
    pilha_push(&e.tab[0], carta_criar(0, 8));
    pilha_push(&e.tab[1], carta_criar(0, 9));
    CU_ASSERT_TRUE(simon_jogar(&e, 0, 1, 1));
    CU_ASSERT_TRUE(pilha_vazia(&e.tab[0]));
    CU_ASSERT_EQUAL(pilha_tamanho(&e.tab[1]), 2);
}

/* verifica que a função não crasha e devolve 0 ou 1 */
void test_simon_tem_jogada(void) {
    SimonEstado e;
    simon_estado_inicializar(&e, 1);
    int res = simon_tem_jogada(&e);
    CU_ASSERT(res == 0 || res == 1);
}

/* verifica que uma sequência K..A completa é movida automaticamente para a fundação */
void test_simon_auto(void) {
    SimonEstado e;
    simon_estado_inicializar(&e, 1);
    /* forçar sequência completa K..A do mesmo naipe na coluna 0 */
    pilha_inicializar(&e.tab[0]);
    for (int v = 13; v >= 1; v--)
        pilha_push(&e.tab[0], carta_criar(0, v));
    int movs = simon_auto(&e);
    CU_ASSERT_TRUE(movs >= 1);
    CU_ASSERT_TRUE(pilha_vazia(&e.tab[0]));
}

int main(void) {
    CU_initialize_registry();

    CU_pSuite s_pilha = CU_add_suite("pilha", NULL, NULL);
    CU_add_test(s_pilha, "inicializar",  test_pilha_inicializar);
    CU_add_test(s_pilha, "push_pop",     test_pilha_push_pop);
    CU_add_test(s_pilha, "carta",        test_pilha_carta);
    CU_add_test(s_pilha, "cheia",        test_pilha_cheia);

    CU_pSuite s_regras = CU_add_suite("simon_regras", NULL, NULL);
    CU_add_test(s_regras, "mesmo_naipe",        test_mesmo_naipe);
    CU_add_test(s_regras, "valor_inferior",     test_valor_inferior);
    CU_add_test(s_regras, "sequencia_valida",   test_sequencia_valida);
    CU_add_test(s_regras, "pode_mover_vazia",   test_pode_mover_para_vazia);
    CU_add_test(s_regras, "pode_mover_valor",   test_pode_mover_valor_inferior);
    CU_add_test(s_regras, "sequencia_completa", test_sequencia_completa);

    CU_pSuite s_estado = CU_add_suite("simon_estado", NULL, NULL);
    CU_add_test(s_estado, "inicializar",      test_simon_inicializar);
    CU_add_test(s_estado, "ganhou",           test_simon_ganhou);
    CU_add_test(s_estado, "pode_jogar_inv",   test_simon_pode_jogar_invalido);
    CU_add_test(s_estado, "jogar",            test_simon_jogar);
    CU_add_test(s_estado, "tem_jogada",       test_simon_tem_jogada);
    CU_add_test(s_estado, "auto",             test_simon_auto);

    CU_basic_set_mode(CU_BRM_VERBOSE);
    CU_basic_run_tests();
    int falhas = (int)CU_get_number_of_failures();
    CU_cleanup_registry();
    return falhas;
}
