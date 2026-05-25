#ifndef PACIENCIA_H
#define PACIENCIA_H

/**
 * @file paciencia.h
 * @brief Estruturas que representam uma paciência descrita pela DSL.
 *
 * Após o parsing de um ficheiro .paciencia, toda a informação fica
 * guardada numa struct Paciencia — tipos de pilha, instâncias, regras
 * de movimento e condições de vitória.
 */

/* ── limites ─────────────────────────────────────────────────── */

/** @brief Tamanho máximo do nome de uma paciência ou tipo de pilha. */
#define PAC_MAX_NOME       32

/** @brief Número máximo de tipos de pilha distintos (TIPO). */
#define PAC_MAX_TIPOS       8

/** @brief Número máximo de instâncias de pilha (INIT). */
#define PAC_MAX_PILHAS     20

/** @brief Número máximo de regras de movimento (MOV + AUTO). */
#define PAC_MAX_REGRAS     16

/** @brief Número máximo de condições de vitória (WIN). */
#define PAC_MAX_WINS        8

/* ── flags de tipo de pilha (TIPO) ───────────────────────────── */

/** @brief Todas as cartas da pilha são visíveis (=). */
#define TIPO_FLAG_TODAS_VISIVEIS  (1 << 0)

/** @brief Nenhuma carta da pilha é visível (_). */
#define TIPO_FLAG_NENHUMA_VISIVEL (1 << 1)

/** @brief Apenas a carta do topo é visível (^). */
#define TIPO_FLAG_SO_TOPO         (1 << 2)

/** @brief A pilha pode conter no máximo uma carta (1). */
#define TIPO_FLAG_MAX_UMA         (1 << 3)

/* ── flags de movimento (MOV / AUTO) ─────────────────────────── */

/** @brief Sem restrições — movimento sempre permitido (*). */
#define MOV_FLAG_SEMPRE      (1 << 0)

/** @brief Pode mover uma sequência de cartas (+). */
#define MOV_FLAG_SEQUENCIA   (1 << 1)

/** @brief Sequência ordenada de forma decrescente ([). */
#define MOV_FLAG_DECRESC     (1 << 2)

/** @brief Sequência ordenada de forma crescente (]). */
#define MOV_FLAG_CRESC       (1 << 3)

/** @brief Valor imediatamente inferior ao topo do destino (<). */
#define MOV_FLAG_INFERIOR    (1 << 4)

/** @brief Valor imediatamente superior ao topo do destino (>). */
#define MOV_FLAG_SUPERIOR    (1 << 5)

/** @brief Valor imediatamente inferior ou superior (~). */
#define MOV_FLAG_ADJACENTE   (1 << 6)

/** @brief Cartas a mover devem ser do mesmo naipe (m). */
#define MOV_FLAG_MESMO_NAIPE (1 << 7)

/** @brief Topo da sequência deve ser do mesmo naipe que topo do destino (M). */
#define MOV_FLAG_MESMO_NAIPE_DEST (1 << 8)

/** @brief Naipes alternados na sequência (x). */
#define MOV_FLAG_NAIPE_ALT   (1 << 9)

/** @brief Topo da sequência deve ser de naipe diferente do topo do destino (X). */
#define MOV_FLAG_NAIPE_DIFER_DEST (1 << 10)

/** @brief Cartas a mover devem ter a mesma cor (c). */
#define MOV_FLAG_MESMA_COR   (1 << 11)

/** @brief Topo da sequência deve ter a mesma cor que topo do destino (C). */
#define MOV_FLAG_MESMA_COR_DEST (1 << 12)

/** @brief Cores alternadas na sequência (d). */
#define MOV_FLAG_COR_ALT     (1 << 13)

/** @brief Topo da sequência deve ter cor diferente do topo do destino (D). */
#define MOV_FLAG_COR_DIFER_DEST (1 << 14)

/** @brief Pilha de destino deve estar vazia (V). */
#define MOV_FLAG_DEST_VAZIA  (1 << 15)

/** @brief Topo da sequência deve ser um Ás (a). */
#define MOV_FLAG_TOPO_AS     (1 << 16)

/** @brief Fundo da sequência deve ser um Ás (A). */
#define MOV_FLAG_FUNDO_AS    (1 << 17)

/** @brief Topo da sequência deve ser um Rei (k). */
#define MOV_FLAG_TOPO_REI    (1 << 18)

/** @brief Fundo da sequência deve ser um Rei (K). */
#define MOV_FLAG_FUNDO_REI   (1 << 19)

/* ── estruturas ──────────────────────────────────────────────── */

/**
 * @brief Representa um tipo de pilha definido pelo comando TIPO.
 *
 * @var TipoPilha::nome   Nome do tipo (ex: "TAB", "STOCK").
 * @var TipoPilha::flags  Combinação de TIPO_FLAG_*.
 */
typedef struct {
    char nome[PAC_MAX_NOME];
    int  flags;
} TipoPilha;

/**
 * @brief Representa uma instância de pilha criada pelo comando INIT.
 *
 * @var InstPilha::idx_tipo      Índice no array de tipos da Paciencia.
 * @var InstPilha::cartas_init   Número de cartas iniciais.
 */
typedef struct {
    int idx_tipo;
    int cartas_init;
} InstPilha;

/**
 * @brief Representa uma regra de movimento (MOV ou AUTO).
 *
 * @var RegraMovimento::idx_origem  Índice do tipo de pilha de origem.
 * @var RegraMovimento::idx_destino Índice do tipo de pilha de destino.
 * @var RegraMovimento::flags       Combinação de MOV_FLAG_*.
 * @var RegraMovimento::automatico  1 se for AUTO, 0 se for MOV.
 */
typedef struct {
    int idx_origem;
    int idx_destino;
    int flags;
    int automatico;
} RegraMovimento;

/**
 * @brief Representa uma condição de vitória (WIN).
 *
 * @var CondicaoVitoria::idx_tipo      Índice do tipo de pilha.
 * @var CondicaoVitoria::num_cartas    Número de cartas exigido em cada pilha desse tipo.
 */
typedef struct {
    int idx_tipo;
    int num_cartas;
} CondicaoVitoria;

/**
 * @brief Estrutura completa de uma paciência após o parsing do ficheiro DSL.
 *
 * @var Paciencia::nome         Nome do jogo (comando JOGO).
 * @var Paciencia::num_baralhos Número de baralhos (comando BARALHOS).
 * @var Paciencia::tipos        Array de tipos de pilha definidos (TIPO).
 * @var Paciencia::num_tipos    Número de tipos definidos.
 * @var Paciencia::pilhas       Array de instâncias de pilha (INIT).
 * @var Paciencia::num_pilhas   Número de instâncias.
 * @var Paciencia::regras       Array de regras de movimento (MOV + AUTO).
 * @var Paciencia::num_regras   Número de regras.
 * @var Paciencia::wins         Array de condições de vitória (WIN).
 * @var Paciencia::num_wins     Número de condições.
 */
typedef struct {
    char            nome[PAC_MAX_NOME];
    int             num_baralhos;

    TipoPilha       tipos[PAC_MAX_TIPOS];
    int             num_tipos;

    InstPilha       pilhas[PAC_MAX_PILHAS];
    int             num_pilhas;

    RegraMovimento  regras[PAC_MAX_REGRAS];
    int             num_regras;

    CondicaoVitoria wins[PAC_MAX_WINS];
    int             num_wins;
} Paciencia;

#endif /* PACIENCIA_H */
