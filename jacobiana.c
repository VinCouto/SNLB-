#include <stdio.h>
#include <stdlib.h>
#include "jacobiana.h"
#include "utils.h"

//Computa o vetor F(X) para o Sistema Não-Linear de Broyden
void avalia_funcao(const real_t *restrict X, real_t *restrict F, int n) {
    //Caso base de segurança
    if (n <= 0) return;

    //f_1(x) = -2*x_1^2 + 3*x_1 - 2*x_2 + 1
    F[0] = -2.0 * X[0] * X[0] + 3.0 * X[0] - 2.0 * X[1] + 1.0;

    //f_i(x) = -2*x_i^2 + 3*x_i - x_{i-1} - 2*x_{i+1} + 1
    //Loop otimizado para caso geral, bom para prefetching de cache
    for (int i = 1; i < n - 1; ++i) {
        F[i] = -2.0 * X[i] * X[i] + 3.0 * X[i] - X[i - 1] - 2.0 * X[i + 1] + 1.0;
    }

    // f_n(x) = -2*x_n^2 + 3*x_n - x_{n-1}
    if (n > 1) {
        F[n - 1] = -2.0 * X[n - 1] * X[n - 1] + 3.0 * X[n - 1] - X[n - 2];
    }
}

//Atualiza apenas a diagonal principal da Jacobiana analítica
void atualiza_diagonal_jacobiana(const real_t *restrict X, real_t *restrict diag_principal, int n) {
    //J_ii = -4 * x_i + 3
    //Loop simples, bom para prefetching de cache
    for (int i = 0; i < n; ++i) {
        diag_principal[i] = -4.0 * X[i] + 3.0;
    }
}
