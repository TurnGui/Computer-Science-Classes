#include <CUnit/CUnit.h>
#include <CUnit/Basic.h>
#include <string.h>
#include <stdio.h>

#include "carta.h"
#include "pilha.h"
#include "paciencia.h"
#include "paciencia_parser.h"
#include "jogo_estado.h"
#include "jogo_saveload.h"

/* ===== paciencia_parser ===== */

/* verifica que o bitmask das flags de MOV é calculado correctamente */
void test_flags_bitmask(void) {
    int f = paciencia_flags_para_bitmask("*");
    CU_ASSERT_TRUE(f & MOV_FLAG_SEMPRE);

    f = paciencia_flags_para_bitmask("+[m<");
    CU_ASSERT_TRUE(f & MOV_FLAG_SEQUENCIA);
    CU_ASSERT_TRUE(f & MOV_FLAG_DECRESC);
    CU_ASSERT_TRUE(f & MOV_FLAG_MESMO_NAIPE);
    CU_ASSERT_TRUE(f & MOV_FLAG_INFERIOR);
    CU_ASSERT_FALSE(f & MOV_FLAG_SUPERIOR);
}

/* verifica que paciencia_indice_tipo devolve o índice correcto */
void test_indice_tipo(void) {
    Paciencia p;
    memset(&p, 0, sizeof(p));
    strncpy(p.tipos[0].nome, "TAB",  PAC_MAX_NOME - 1);
    strncpy(p.tipos[1].nome, "FUND", PAC_MAX_NOME - 1);
    p.num_tipos = 2;

    CU_ASSERT_EQUAL(paciencia_indice_tipo(&p, "TAB"),  0);
    CU_ASSERT_EQUAL(paciencia_indice_tipo(&p, "FUND"), 1);
    CU_ASSERT_EQUAL(paciencia_indice_tipo(&p, "XXX"), -1);
}

/* verifica que o parser lê correctamente o ficheiro do Golf */
void test_parser_golf(void) {
    Paciencia p;
    int ok = paciencia_ler("paciencias/golf.paciencia", &p);
    CU_ASSERT_TRUE(ok);
    CU_ASSERT_STRING_EQUAL(p.nome, "Golf");
    CU_ASSERT_EQUAL(p.num_baralhos, 1);
    CU_ASSERT_TRUE(p.num_tipos   > 0);
    CU_ASSERT_TRUE(p.num_pilhas  > 0);
    CU_ASSERT_TRUE(p.num_regras  > 0);
    CU_ASSERT_TRUE(p.num_wins    > 0);
}

/* verifica que o parser lê correctamente o ficheiro do Simple Simon */
void test_parser_simon(void) {
    Paciencia p;
    int ok = paciencia_ler("paciencias/simon.paciencia", &p);
    CU_ASSERT_TRUE(ok);
    CU_ASSERT_STRING_EQUAL(p.nome, "SimpleSimon");
    CU_ASSERT_EQUAL(p.num_baralhos, 1);
}

/* verifica que o parser devolve 0 para um ficheiro inexistente */
void test_parser_ficheiro_invalido(void) {
    Paciencia p;
    CU_ASSERT_FALSE(paciencia_ler("paciencias/nao_existe.paciencia", &p));
}

/* ===== jogo_estado ===== */

/**
 * @brief Cria uma Paciencia mínima para testes (2 tipos, 2 pilhas, 1 regra MOV *).
 */
static void paciencia_minima(Paciencia *p) {
    memset(p, 0, sizeof(Paciencia));
    strncpy(p->nome, "Teste", PAC_MAX_NOME - 1);
    p->num_baralhos = 1;

    /* tipos: TAB e FUND */
    strncpy(p->tipos[0].nome, "TAB",  PAC_MAX_NOME - 1);
    p->tipos[0].flags = TIPO_FLAG_TODAS_VISIVEIS;
    strncpy(p->tipos[1].nome, "FUND", PAC_MAX_NOME - 1);
    p->tipos[1].flags = TIPO_FLAG_TODAS_VISIVEIS;
    p->num_tipos = 2;

    /* 2 pilhas TAB com 5 cartas cada */
    p->pilhas[0].idx_tipo = 0; p->pilhas[0].cartas_init = 5;
    p->pilhas[1].idx_tipo = 0; p->pilhas[1].cartas_init = 5;
    p->num_pilhas = 2;

    /* regra MOV TAB TAB * (sempre permitido) */
    p->regras[0].idx_origem  = 0;
    p->regras[0].idx_destino = 0;
    p->regras[0].flags       = MOV_FLAG_SEMPRE;
    p->regras[0].automatico  = 0;
    p->num_regras = 1;

    /* vitória: TAB com 0 cartas */
    p->wins[0].idx_tipo   = 0;
    p->wins[0].num_cartas = 0;
    p->num_wins = 1;
}

/* verifica que o estado é inicializado com as pilhas certas */
void test_jogo_inicializar(void) {
    Paciencia pac;
    paciencia_minima(&pac);
    JogoEstado e;
    jogo_estado_inicializar(&e, &pac, 42);
    CU_ASSERT_EQUAL(e.num_pilhas, 2);
    CU_ASSERT_EQUAL(pilha_tamanho(&e.pilhas[0]), 5);
    CU_ASSERT_EQUAL(pilha_tamanho(&e.pilhas[1]), 5);
}

/* verifica vitória quando todas as pilhas TAB estão vazias */
void test_jogo_ganhou(void) {
    Paciencia pac;
    paciencia_minima(&pac);
    JogoEstado e;
    jogo_estado_inicializar(&e, &pac, 1);
    CU_ASSERT_FALSE(jogo_ganhou(&e));
    pilha_inicializar(&e.pilhas[0]);
    pilha_inicializar(&e.pilhas[1]);
    CU_ASSERT_TRUE(jogo_ganhou(&e));
}

/* verifica que um movimento sempre permitido funciona */
void test_jogo_mover(void) {
    Paciencia pac;
    paciencia_minima(&pac);
    JogoEstado e;
    jogo_estado_inicializar(&e, &pac, 1);
    int t0 = pilha_tamanho(&e.pilhas[0]);
    int t1 = pilha_tamanho(&e.pilhas[1]);
    CU_ASSERT_TRUE(jogo_mover(&e, 0, 1, 1));
    CU_ASSERT_EQUAL(pilha_tamanho(&e.pilhas[0]), t0 - 1);
    CU_ASSERT_EQUAL(pilha_tamanho(&e.pilhas[1]), t1 + 1);
}

/* verifica que índices inválidos são rejeitados */
void test_jogo_mover_invalido(void) {
    Paciencia pac;
    paciencia_minima(&pac);
    JogoEstado e;
    jogo_estado_inicializar(&e, &pac, 1);
    CU_ASSERT_FALSE(jogo_mover(&e, -1, 0, 1));
    CU_ASSERT_FALSE(jogo_mover(&e,  0, 0, 1));
    CU_ASSERT_FALSE(jogo_mover(&e,  0, 5, 1));
}

/* ===== undo ===== */

/* verifica que guardar e restaurar funciona correctamente */
void test_undo(void) {
    Paciencia pac;
    paciencia_minima(&pac);
    JogoEstado e;
    jogo_estado_inicializar(&e, &pac, 1);

    JogoHistorico h;
    jogo_historico_inicializar(&h);

    int t0 = pilha_tamanho(&e.pilhas[0]);
    jogo_historico_guardar(&h, &e);
    jogo_mover(&e, 0, 1, 1);
    CU_ASSERT_EQUAL(pilha_tamanho(&e.pilhas[0]), t0 - 1);

    jogo_historico_restaurar(&h, &e);
    CU_ASSERT_EQUAL(pilha_tamanho(&e.pilhas[0]), t0);
}

/* verifica que restaurar com histórico vazio devolve 0 */
void test_undo_vazio(void) {
    JogoHistorico h;
    jogo_historico_inicializar(&h);
    Paciencia pac;
    paciencia_minima(&pac);
    JogoEstado e;
    jogo_estado_inicializar(&e, &pac, 1);
    CU_ASSERT_FALSE(jogo_historico_restaurar(&h, &e));
}

/* ===== save/load ===== */

/* verifica que gravar e carregar preserva o estado */
void test_saveload(void) {
    Paciencia pac;
    paciencia_minima(&pac);
    JogoEstado e1, e2;
    jogo_estado_inicializar(&e1, &pac, 7);
    jogo_estado_inicializar(&e2, &pac, 7);

    CU_ASSERT_TRUE(jogo_gravar("test_save.tmp", &e1, "teste.paciencia"));

    /* modificar e2 para confirmar que o load repõe e1 */
    jogo_mover(&e2, 0, 1, 1);
    CU_ASSERT_TRUE(jogo_carregar("test_save.tmp", &e2));

    CU_ASSERT_EQUAL(pilha_tamanho(&e2.pilhas[0]),
                    pilha_tamanho(&e1.pilhas[0]));
    CU_ASSERT_EQUAL(pilha_tamanho(&e2.pilhas[1]),
                    pilha_tamanho(&e1.pilhas[1]));
    remove("test_save.tmp");
}

/* ===== main ===== */

int main(void) {
    CU_initialize_registry();

    CU_pSuite s_parser = CU_add_suite("paciencia_parser", NULL, NULL);
    CU_add_test(s_parser, "flags_bitmask",       test_flags_bitmask);
    CU_add_test(s_parser, "indice_tipo",          test_indice_tipo);
    CU_add_test(s_parser, "parser_golf",          test_parser_golf);
    CU_add_test(s_parser, "parser_simon",         test_parser_simon);
    CU_add_test(s_parser, "parser_fich_invalido", test_parser_ficheiro_invalido);

    CU_pSuite s_estado = CU_add_suite("jogo_estado", NULL, NULL);
    CU_add_test(s_estado, "inicializar",    test_jogo_inicializar);
    CU_add_test(s_estado, "ganhou",         test_jogo_ganhou);
    CU_add_test(s_estado, "mover",          test_jogo_mover);
    CU_add_test(s_estado, "mover_invalido", test_jogo_mover_invalido);

    CU_pSuite s_undo = CU_add_suite("undo", NULL, NULL);
    CU_add_test(s_undo, "undo",       test_undo);
    CU_add_test(s_undo, "undo_vazio", test_undo_vazio);

    CU_pSuite s_save = CU_add_suite("saveload", NULL, NULL);
    CU_add_test(s_save, "saveload", test_saveload);

    CU_basic_set_mode(CU_BRM_VERBOSE);
    CU_basic_run_tests();
    int falhas = (int)CU_get_number_of_failures();
    CU_cleanup_registry();
    return falhas;
}
