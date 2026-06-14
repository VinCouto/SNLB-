#ifndef __JACOBIANA_H__
#define __JACOBIANA_H__

#include "utils.h"

/**
 * @brief Computa o vetor de funções F(X) do Sistema Não-Linear de Broyden.
 * * @param X Vetor de variáveis atual (tamanho n).
 * @param F Vetor que armazenará o resultado de F(X) (tamanho n).
 * @param n Dimensão do sistema.
 */
void avalia_funcao(const real_t *X, real_t *F, int n);

/**
 * @brief Atualiza os valores da diagonal principal da Matriz Jacobiana.
 * * @param X Vetor de variáveis atual (tamanho n).
 * @param diag_principal Vetor que armazenará a diagonal principal (tamanho n).
 * @param n Dimensão do sistema.
 */
void atualiza_diagonal_jacobiana(const real_t *X, real_t *diag_principal, int n);

#endif // __JACOBIANA_H__