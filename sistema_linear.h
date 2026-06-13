#ifndef __SISTEMA_LINEAR_H__
#define __SISTEMA_LINEAR_H__

#include "utils.h"

/**
 * @brief Resolve um sistema linear tridiagonal J * dX = -F utilizando o Algoritmo de Thomas.
 *
 * Devido à estrutura do SNLB de Broyden, a subdiagonal e a superdiagonal são constantes.
 * Para otimizar a largura de banda de memória e o uso de cache, os coeficientes das 
 * diagonais secundárias são aplicados diretamente de forma analítica (sub = -1.0, super = -2.0),
 * eliminando a necessidade de ler arrays extras para essas diagonais.
 *
 * O vetor F entra contendo o resíduo F(X) e sai contendo a própria solução dX (passo de Newton).
 *
 * @param diag_principal Ponteiro alinhado para o vetor contendo a diagonal principal (tamanho n).
 * @param F Ponteiro alinhado para o vetor de termos independentes / saída do passo (tamanho n).
 * @param n Dimensão do sistema linear.
 * @return int Retorna 0 em caso de sucesso, ou -1 se houver divisão por zero (matriz singular).
 */
int resolve_sistema_tridiagonal(const real_t *restrict diag_principal, real_t *restrict F, int n);

#endif // __SISTEMA_LINEAR_H__