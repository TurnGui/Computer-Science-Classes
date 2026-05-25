#ifndef PACIENCIA_PARSER_H
#define PACIENCIA_PARSER_H

/**
 * @file paciencia_parser.h
 * @brief Parser de ficheiros DSL de paciências.
 *
 * Lê um ficheiro .paciencia e preenche uma struct Paciencia.
 */

#include "paciencia.h"

/**
 * @brief Lê um ficheiro DSL e preenche a struct Paciencia.
 *
 * @param caminho Caminho para o ficheiro .paciencia.
 * @param p       Apontador para a struct a preencher.
 * @return 1 se o parsing foi bem-sucedido, 0 em caso de erro.
 */
int paciencia_ler(const char *caminho, Paciencia *p);

/**
 * @brief Converte uma string de flags (DSL) num bitmask inteiro.
 *
 * Percorre cada caractere da string e activa o bit correspondente.
 *
 * @param flags_str String de flags (ex: "+[m<").
 * @return Bitmask com os flags activos.
 */
int paciencia_flags_para_bitmask(const char *flags_str);

/**
 * @brief Procura o índice de um tipo de pilha pelo nome.
 *
 * @param p    Apontador para a struct Paciencia.
 * @param nome Nome do tipo a procurar (ex: "TAB").
 * @return Índice no array tipos[], ou -1 se não encontrado.
 */
int paciencia_indice_tipo(const Paciencia *p, const char *nome);

#endif /* PACIENCIA_PARSER_H */
