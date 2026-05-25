#ifndef JOGO_SAVELOAD_H
#define JOGO_SAVELOAD_H

/**
 * @file jogo_saveload.h
 * @brief Gravação e carregamento de estados de jogo.
 */

#include "jogo_estado.h"

/**
 * @brief Grava o estado actual num ficheiro.
 *
 * @param caminho  Caminho do ficheiro de destino.
 * @param e        Estado a gravar.
 * @param pac_nome Nome do ficheiro da paciência (primeira linha).
 * @return 1 se bem-sucedido, 0 em caso de erro.
 */
int jogo_gravar(const char *caminho, const JogoEstado *e, const char *pac_nome);

/**
 * @brief Carrega um estado de jogo a partir de um ficheiro.
 *
 * @param caminho Caminho do ficheiro a ler.
 * @param e       Estado a preencher.
 * @return 1 se bem-sucedido, 0 em caso de erro.
 */
int jogo_carregar(const char *caminho, JogoEstado *e);

#endif /* JOGO_SAVELOAD_H */
