#ifndef JOGO_ESTADO_H
#define JOGO_ESTADO_H

/**
 * @file jogo_estado.h
 * @brief Estado genérico de uma partida de qualquer paciência descrita pela DSL.
 */

#include "paciencia.h"
#include "pilha.h"
#include "baralho.h"

/** @brief Número máximo de estados guardados para undo. */
#define MAX_UNDO 64

/**
 * @brief Estado completo de uma partida em curso.
 *
 * @var JogoEstado::pilhas      Array de pilhas activas (uma por INIT).
 * @var JogoEstado::num_pilhas  Número de pilhas activas.
 * @var JogoEstado::pac         Apontador para a descrição da paciência.
 */
typedef struct {
    Pilha       pilhas[PAC_MAX_PILHAS];
    int         num_pilhas;
    const Paciencia *pac;
} JogoEstado;

/**
 * @brief Snapshot do estado das pilhas para undo.
 *
 * Guarda apenas o array de pilhas — o suficiente para restaurar.
 *
 * @var JogoSnapshot::pilhas     Cópia das pilhas no momento do snapshot.
 * @var JogoSnapshot::num_pilhas Número de pilhas.
 */
typedef struct {
    Pilha pilhas[PAC_MAX_PILHAS];
    int   num_pilhas;
} JogoSnapshot;

/**
 * @brief Histórico de snapshots para suportar undo.
 *
 * @var JogoHistorico::snapshots  Array de snapshots.
 * @var JogoHistorico::topo       Índice do topo (-1 = vazio).
 */
typedef struct {
    JogoSnapshot snapshots[MAX_UNDO];
    int          topo;
} JogoHistorico;

/* ── funções ─────────────────────────────────────────────────── */

/**
 * @brief Inicializa o estado do jogo a partir de uma paciência e uma seed.
 *
 * Cria e embaralha o(s) baralho(s), distribui as cartas pelas pilhas
 * segundo os comandos INIT da paciência.
 *
 * @param e    Apontador para o estado a inicializar.
 * @param pac  Apontador para a paciência já parseada.
 * @param seed Semente para o gerador pseudo-aleatório.
 */
void jogo_estado_inicializar(JogoEstado *e, const Paciencia *pac, unsigned seed);

/**
 * @brief Verifica se as condições de vitória (WIN) estão todas satisfeitas.
 *
 * @param e Apontador constante para o estado.
 * @return 1 se ganhou, 0 caso contrário.
 */
int jogo_ganhou(const JogoEstado *e);

/**
 * @brief Verifica se um movimento de n cartas de uma pilha para outra é válido.
 *
 * Percorre as regras MOV da paciência e verifica se alguma se aplica.
 *
 * @param e    Apontador constante para o estado.
 * @param orig Índice da pilha de origem.
 * @param dest Índice da pilha de destino.
 * @param n    Número de cartas a mover.
 * @return 1 se válido, 0 caso contrário.
 */
int jogo_pode_mover(const JogoEstado *e, int orig, int dest, int n);

/**
 * @brief Executa o movimento de n cartas de uma pilha para outra.
 *
 * @param e    Apontador para o estado.
 * @param orig Índice da pilha de origem.
 * @param dest Índice da pilha de destino.
 * @param n    Número de cartas a mover.
 * @return 1 se executado, 0 caso contrário.
 */
int jogo_mover(JogoEstado *e, int orig, int dest, int n);

/**
 * @brief Executa todos os movimentos automáticos (AUTO) possíveis.
 *
 * Corre em loop até não encontrar mais AUTO aplicáveis.
 *
 * @param e Apontador para o estado.
 * @return Número de movimentos automáticos realizados.
 */
int jogo_auto(JogoEstado *e);

/**
 * @brief Verifica se existe pelo menos um movimento válido (MOV).
 *
 * @param e Apontador constante para o estado.
 * @return 1 se existe jogada, 0 caso contrário.
 */
int jogo_tem_jogada(const JogoEstado *e);

/* ── undo ────────────────────────────────────────────────────── */

/**
 * @brief Inicializa o histórico de undo (vazio).
 * @param h Apontador para o histórico.
 */
void jogo_historico_inicializar(JogoHistorico *h);

/**
 * @brief Guarda o estado actual no histórico.
 *
 * @param h Apontador para o histórico.
 * @param e Apontador constante para o estado actual.
 * @return 1 se guardou, 0 se o histórico está cheio.
 */
int jogo_historico_guardar(JogoHistorico *h, const JogoEstado *e);

/**
 * @brief Restaura o último estado guardado (undo).
 *
 * @param h Apontador para o histórico.
 * @param e Apontador para o estado a restaurar.
 * @return 1 se restaurou, 0 se o histórico está vazio.
 */
int jogo_historico_restaurar(JogoHistorico *h, JogoEstado *e);

#endif /* JOGO_ESTADO_H */
