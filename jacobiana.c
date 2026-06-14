#include <stdio.h>
#include <stdlib.h>
#include "jacobiana.h"

void avalia_funcao(real_t *X, real_t *F, int n) {
    if (n <= 0) return;
    F[0] = -2.0 * X[0] * X[0] + 3.0 * X[0] - 2.0 * X[1] + 1.0;
    for (int i = 1; i < n - 1; ++i) {
        F[i] = -2.0 * X[i] * X[i] + 3.0 * X[i] - X[i - 1] - 2.0 * X[i + 1] + 1.0;
    }
    if (n > 1) {
        F[n - 1] = -2.0 * X[n - 1] * X[n - 1] + 3.0 * X[n - 1] - X[n - 2];
    }
}

// Preenche a matriz Jacobiana inteira (O(N^2))
void atualiza_jacobiana(real_t *X, real_t **J, int n) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            J[i][j] = 0.0; // Preenchimento ineficiente de zeros
        }
    }
    
    for (int i = 0; i < n; i++) {
        J[i][i] = -4.0 * X[i] + 3.0;
        if (i > 0) J[i][i - 1] = -1.0;
        if (i < n - 1) J[i][i + 1] = -2.0;
    }
}