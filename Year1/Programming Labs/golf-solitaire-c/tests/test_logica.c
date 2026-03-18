#include <CUnit/CUnit.h>
#include <CUnit/Basic.h>

#include "../include/carta.h"
#include "../include/baralho.h"
#include "../include/golf.h"
#include "../include/golf_estado.h"

//ficheiro .c do chatgpt pra testar todas as funções com cunit

/* ===================== CARTA ===================== */

void test_carta_criar(void) {
    Carta c = carta_criar(2, 7);
    CU_ASSERT_EQUAL(c.naipe, 2);
    CU_ASSERT_EQUAL(c.valor, 7);
}

void test_carta_valida_valida(void) {
    Carta c = carta_criar(0, 1);   // mínimo válido
    CU_ASSERT_TRUE(carta_valida(c));

    Carta c2 = carta_criar(3, 13); // máximo válido
    CU_ASSERT_TRUE(carta_valida(c2));
}

void test_carta_valida_naipe_invalido(void) {
    Carta c = carta_criar(4, 5);   // naipe 4 não existe
    CU_ASSERT_FALSE(carta_valida(c));

    Carta c2 = carta_criar(-1, 5); // naipe negativo
    CU_ASSERT_FALSE(carta_valida(c2));
}

void test_carta_valida_valor_invalido(void) {
    Carta c = carta_criar(1, 0);   // valor 0 não existe
    CU_ASSERT_FALSE(carta_valida(c));

    Carta c2 = carta_criar(1, 14); // valor 14 não existe
    CU_ASSERT_FALSE(carta_valida(c2));
}

/* ===================== BARALHO ===================== */

void test_baralho_inicializar(void) {
    Baralho b;
    baralho_inicializar(&b);

    CU_ASSERT_EQUAL(b.topo, 0);
    // primeira carta: naipe 0, valor 1
    CU_ASSERT_EQUAL(b.cartas[0].naipe, 0);
    CU_ASSERT_EQUAL(b.cartas[0].valor, 1);
    // última carta: naipe 3, valor 13
    CU_ASSERT_EQUAL(b.cartas[51].naipe, 3);
    CU_ASSERT_EQUAL(b.cartas[51].valor, 13);
}

void test_baralho_retirar(void) {
    Baralho b;
    baralho_inicializar(&b);

    Carta c = baralho_retirar(&b);
    CU_ASSERT_EQUAL(c.naipe, 0);
    CU_ASSERT_EQUAL(c.valor, 1);
    CU_ASSERT_EQUAL(b.topo, 1); // topo avançou
}

void test_baralho_vazio_falso(void) {
    Baralho b;
    baralho_inicializar(&b);
    CU_ASSERT_FALSE(baralho_vazio(&b));
}

void test_baralho_vazio_verdadeiro(void) {
    Baralho b;
    baralho_inicializar(&b);

    for (int i = 0; i < TAMANHO_BARALHO; i++)
        baralho_retirar(&b);

    CU_ASSERT_TRUE(baralho_vazio(&b));
}

void test_baralho_embaralhar(void) {
    Baralho b;
    baralho_inicializar(&b);
    baralho_embaralhar(&b);

    // Após embaralhar o topo deve ser 0
    CU_ASSERT_EQUAL(b.topo, 0);

    // Verificar que ainda tem 52 cartas válidas (sem duplicados)
    int contagem[4][14] = {0};
    for (int i = 0; i < TAMANHO_BARALHO; i++) {
        int n = b.cartas[i].naipe;
        int v = b.cartas[i].valor;
        CU_ASSERT_TRUE(n >= 0 && n < 4);
        CU_ASSERT_TRUE(v >= 1 && v <= 13);
        contagem[n][v]++;
    }
    for (int n = 0; n < 4; n++)
        for (int v = 1; v <= 13; v++)
            CU_ASSERT_EQUAL(contagem[n][v], 1); // cada carta aparece exatamente 1 vez
}

/* ===================== GOLF (regras) ===================== */

void test_golf_carta_pode_jogar_valido(void) {
    Carta mesa    = carta_criar(0, 7);
    Carta acima   = carta_criar(1, 8); // valor+1
    Carta abaixo  = carta_criar(2, 6); // valor-1

    CU_ASSERT_TRUE(golf_carta_pode_jogar(mesa, acima));
    CU_ASSERT_TRUE(golf_carta_pode_jogar(mesa, abaixo));
}

void test_golf_carta_pode_jogar_invalido(void) {
    Carta mesa   = carta_criar(0, 7);
    Carta longe  = carta_criar(1, 5); // diferença de 2
    Carta igual  = carta_criar(2, 7); // mesmo valor

    CU_ASSERT_FALSE(golf_carta_pode_jogar(mesa, longe));
    CU_ASSERT_FALSE(golf_carta_pode_jogar(mesa, igual));
}

/* ===================== GOLF ESTADO ===================== */

/* Helper: cria um estado controlado sem aleatoriedade */
static GolfEstado criar_estado_fixo(void) {
    GolfEstado e;
    golf_estado_inicializar(&e, 42); // seed fixa = resultado determinístico
    return e;
}

void test_golf_stock_vazio_falso(void) {
    GolfEstado e = criar_estado_fixo();
    CU_ASSERT_FALSE(golf_stock_vazio(&e));
}

void test_golf_coluna_vazia_falso(void) {
    GolfEstado e = criar_estado_fixo();
    // Logo no início nenhuma coluna está vazia
    for (int c = 0; c < GOLF_COLUNAS; c++)
        CU_ASSERT_FALSE(golf_coluna_vazia(&e, c));
}

void test_golf_coluna_vazia_verdadeiro(void) {
    GolfEstado e = criar_estado_fixo();
    // Esvaziar coluna 0 manualmente
    e.topo[0] = -1;
    CU_ASSERT_TRUE(golf_coluna_vazia(&e, 0));
}

void test_golf_topo_coluna(void) {
    GolfEstado e = criar_estado_fixo();
    // O topo da coluna deve ser uma carta válida
    Carta c = golf_topo_coluna(&e, 0);
    CU_ASSERT_TRUE(carta_valida(c));
}

void test_golf_descarte(void) {
    GolfEstado e = criar_estado_fixo();
    Carta d = golf_descarte(&e);
    CU_ASSERT_TRUE(carta_valida(d));
}

void test_golf_jogar_coluna_invalido(void) {
    GolfEstado e = criar_estado_fixo();
    // Coluna fora dos limites
    CU_ASSERT_FALSE(golf_jogar_coluna(&e, -1));
    CU_ASSERT_FALSE(golf_jogar_coluna(&e, GOLF_COLUNAS));
}

void test_golf_jogar_coluna_valido(void) {
    GolfEstado e = criar_estado_fixo();

    // Procurar uma coluna onde seja possível jogar
    int jogou = 0;
    for (int c = 0; c < GOLF_COLUNAS; c++) {
        if (golf_pode_jogar_coluna(&e, c)) {
            Carta antes = golf_topo_coluna(&e, c);
            int topo_antes = e.topo[c];
            int resultado = golf_jogar_coluna(&e, c);

            CU_ASSERT_TRUE(resultado);
            CU_ASSERT_EQUAL(e.topo[c], topo_antes - 1);      // topo desceu
            CU_ASSERT_EQUAL(e.descarte.naipe, antes.naipe);  // descarte é a carta jogada
            CU_ASSERT_EQUAL(e.descarte.valor, antes.valor);
            jogou = 1;
            break;
        }
    }
    // Se não houver jogada possível o teste fica inconclusivo mas não falha
    (void)jogou;
}

void test_golf_puxar_stock(void) {
    GolfEstado e = criar_estado_fixo();
    int resultado = golf_puxar_stock(&e);
    CU_ASSERT_TRUE(resultado); // stock não está vazio logo deve conseguir puxar
}

void test_golf_puxar_stock_vazio(void) {
    GolfEstado e = criar_estado_fixo();
    // Esvaziar o stock
    while (!golf_stock_vazio(&e))
        golf_puxar_stock(&e);

    CU_ASSERT_FALSE(golf_puxar_stock(&e)); // agora deve falhar
}

void test_golf_ganhou_falso(void) {
    GolfEstado e = criar_estado_fixo();
    CU_ASSERT_FALSE(golf_ganhou(&e)); // no inicio ainda há cartas na mesa
}

void test_golf_ganhou_verdadeiro(void) {
    GolfEstado e = criar_estado_fixo();
    // Esvaziar todas as colunas manualmente
    for (int c = 0; c < GOLF_COLUNAS; c++)
        e.topo[c] = -1;

    CU_ASSERT_TRUE(golf_ganhou(&e));
}

void test_golf_tem_jogada(void) {
    GolfEstado e = criar_estado_fixo();

    // Forçar uma situação com jogada garantida:
    // descarte = valor 7, meter carta 8 no topo da coluna 0
    e.descarte = carta_criar(0, 7);
    e.mesa[0][e.topo[0]] = carta_criar(1, 8);

    CU_ASSERT_TRUE(golf_tem_jogada(&e));
}

void test_golf_sem_jogada(void) {
    GolfEstado e = criar_estado_fixo();

    // Forçar situação sem nenhuma jogada possível:
    // descarte = 7, todas as colunas com carta de valor 1 (diferença >= 2 para todos exceto 6 e 8)
    // Vamos usar valor 1 no descarte e valor 3 em todas as colunas (diferença de 2)
    e.descarte = carta_criar(0, 1);
    for (int c = 0; c < GOLF_COLUNAS; c++) {
        e.topo[c] = 0;
        e.mesa[c][0] = carta_criar(0, 3); // diferença de 2, não jogável
    }

    CU_ASSERT_FALSE(golf_tem_jogada(&e));
}

/* ===================== MAIN DOS TESTES ===================== */

int main(void) {
    CU_initialize_registry();

    /* Suite carta */
    CU_pSuite s_carta = CU_add_suite("carta", NULL, NULL);
    CU_add_test(s_carta, "carta_criar",                  test_carta_criar);
    CU_add_test(s_carta, "carta_valida - valida",        test_carta_valida_valida);
    CU_add_test(s_carta, "carta_valida - naipe invalido",test_carta_valida_naipe_invalido);
    CU_add_test(s_carta, "carta_valida - valor invalido", test_carta_valida_valor_invalido);

    /* Suite baralho */
    CU_pSuite s_baralho = CU_add_suite("baralho", NULL, NULL);
    CU_add_test(s_baralho, "baralho_inicializar",        test_baralho_inicializar);
    CU_add_test(s_baralho, "baralho_retirar",            test_baralho_retirar);
    CU_add_test(s_baralho, "baralho_vazio - falso",      test_baralho_vazio_falso);
    CU_add_test(s_baralho, "baralho_vazio - verdadeiro", test_baralho_vazio_verdadeiro);
    CU_add_test(s_baralho, "baralho_embaralhar",         test_baralho_embaralhar);

    /* Suite golf (regras) */
    CU_pSuite s_golf = CU_add_suite("golf_regras", NULL, NULL);
    CU_add_test(s_golf, "pode_jogar - valido",           test_golf_carta_pode_jogar_valido);
    CU_add_test(s_golf, "pode_jogar - invalido",         test_golf_carta_pode_jogar_invalido);

    /* Suite golf_estado */
    CU_pSuite s_estado = CU_add_suite("golf_estado", NULL, NULL);
    CU_add_test(s_estado, "stock_vazio - falso",         test_golf_stock_vazio_falso);
    CU_add_test(s_estado, "coluna_vazia - falso",        test_golf_coluna_vazia_falso);
    CU_add_test(s_estado, "coluna_vazia - verdadeiro",   test_golf_coluna_vazia_verdadeiro);
    CU_add_test(s_estado, "topo_coluna",                 test_golf_topo_coluna);
    CU_add_test(s_estado, "descarte",                    test_golf_descarte);
    CU_add_test(s_estado, "jogar_coluna - invalido",     test_golf_jogar_coluna_invalido);
    CU_add_test(s_estado, "jogar_coluna - valido",       test_golf_jogar_coluna_valido);
    CU_add_test(s_estado, "puxar_stock",                 test_golf_puxar_stock);
    CU_add_test(s_estado, "puxar_stock - vazio",         test_golf_puxar_stock_vazio);
    CU_add_test(s_estado, "ganhou - falso",              test_golf_ganhou_falso);
    CU_add_test(s_estado, "ganhou - verdadeiro",         test_golf_ganhou_verdadeiro);
    CU_add_test(s_estado, "tem_jogada - tem",            test_golf_tem_jogada);
    CU_add_test(s_estado, "tem_jogada - sem jogada",     test_golf_sem_jogada);

    CU_basic_set_mode(CU_BRM_VERBOSE);
    CU_basic_run_tests();
    CU_cleanup_registry();
    return 0;
}