#ifndef __SISTEMA_LINEAR_H__
#define __SISTEMA_LINEAR_H__

#include "utils.h"

/**
 * @brief Resolve um sistema linear tridiagonal J * dX = -F utilizando o Algoritmo de Thomas.
 *
 * @param diag_principal Ponteiro alinhado para o vetor contendo a diagonal principal (tamanho n).
 * @param F Ponteiro alinhado para o vetor de termos independentes / saída do passo (tamanho n).
 * @param n Dimensão do sistema linear.
 * @return int Retorna 0 em caso de sucesso, ou -1 se houver divisão por zero (matriz singular).
 */
int resolve_sistema_tridiagonal(const real_t *restrict diag_principal, real_t *restrict F, int n);

#endif // __SISTEMA_LINEAR_H__