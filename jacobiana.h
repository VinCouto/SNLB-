#ifndef __JACOBIANA_H__
#define __JACOBIANA_H__

#include "utils.h"

/**
 * @brief Computa o vetor de funções F(X) do Sistema Não-Linear de Broyden.
 * * Esta função avalia cada equação f_i(x) com base no vetor atual X
 * e armazena o resultado no vetor F.
 * * @param X Vetor de variáveis atual (tamanho n).
 * @param F Vetor que armazenará o resultado de F(X) (tamanho n).
 * @param n Dimensão do sistema.
 */
void avalia_funcao(real_t *X, real_t *F, int n);

/**
 * @brief Atualiza os valores da diagonal principal da Matriz Jacobiana.
 * * Como a estrutura do SNLB é tridiagonal e as diagonais secundárias são fixas
 * (subdiagonal = -1.0 e superdiagonal = -2.0), só precisamos computar e
 * atualizar analiticamente a diagonal principal: J_ii = -4 * x_i + 3.
 * * @param X Vetor de variáveis atual (tamanho n).
 * @param diag_principal Vetor que armazenará a diagonal principal (tamanho n).
 * @param n Dimensão do sistema.
 */
void atualiza_jacobiana(real_t *X, real_t **J, int n);

#endif // __JACOBIANA_H__