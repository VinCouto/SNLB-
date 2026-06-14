#include <stdio.h>
#include <stdlib.h>
#include "sistema_linear.h"

// Eliminação de Gauss Clássica (Muito lenta para N grande)
int resolve_sistema_linear(real_t **J, real_t *F, real_t *dX, int n) {
    // Vetor de termos independentes (-F)
    for (int i = 0; i < n; ++i) {
        dX[i] = -F[i];
    }

    // Triangularização O(N^3)
    for (int k = 0; k < n - 1; ++k) {
        if (J[k][k] == 0.0) return -1;
        
        for (int i = k + 1; i < n; ++i) {
            real_t m = J[i][k] / J[k][k];
            J[i][k] = 0.0;
            for (int j = k + 1; j < n; ++j) {
                J[i][j] -= m * J[k][j];
            }
            dX[i] -= m * dX[k];
        }
    }

    // Substituição Retroativa
    if (J[n - 1][n - 1] == 0.0) return -1;
    
    dX[n - 1] = dX[n - 1] / J[n - 1][n - 1];
    for (int i = n - 2; i >= 0; --i) {
        real_t soma = dX[i];
        for (int j = i + 1; j < n; ++j) {
            soma -= J[i][j] * dX[j];
        }
        dX[i] = soma / J[i][i];
    }

    return 0;
}