#ifndef __NEWTON_H__
#define __NEWTON_H__

#include <stdio.h>
#include "utils.h"

/**
 * @brief Executa o Método de Newton para resolver o Sistema Não-Linear de Broyden.
 * * Esta função controla o laço iterativo principal. A cada iteração ela:
 * 1. Avalia o vetor F(X).
 * 2. Verifica os critérios de parada (max_iter e epsilon).
 * 3. Imprime o estado atual das incógnitas no formato exigido.
 * 4. Calcula a nova diagonal da matriz Jacobiana (computando o tempo).
 * 5. Resolve o sistema linear tridiagonal para achar dX (computando o tempo).
 * 6. Atualiza o vetor X (X = X + dX).
 * * @param n Dimensão do sistema.
 * @param X Vetor de variáveis (aproximação inicial e saída da solução).
 * @param F Vetor auxiliar para armazenar F(X) e o passo dX.
 * @param diag_principal Vetor auxiliar para armazenar a diagonal da Jacobiana.
 * @param epsilon Tolerância para o critério de parada.
 * @param max_iter Número máximo de iterações permitido.
 * @param saida Ponteiro de arquivo (stdout ou arquivo .txt) para escrita dos resultados.
 * @param t_jacobiana Ponteiro para acumular o tempo total gasto nas Jacobianas.
 * @param t_sl Ponteiro para acumular o tempo total gasto no Solucionador Linear.
 * @return int Retorna 0 em caso de convergência ou sucesso, ou -1 se houver erro matemático.
 */

int executa_newton(int n, real_t *X, real_t *F, real_t *dX, real_t **J,
                   real_t epsilon, int max_iter, FILE *saida, 
                   double *t_jacobiana, double *t_sl);

#endif // __NEWTON_H__ 