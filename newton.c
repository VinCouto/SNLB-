#include <stdio.h>
#include <math.h> // Usando math.h em vez da macro ABS otimizada
#include <likwid.h>
#include "newton.h"
#include "jacobiana.h"
#include "sistema_linear.h"

int executa_newton(int n, real_t *X, real_t *F, real_t *dX, real_t **J,
                   real_t epsilon, int max_iter, FILE *saida, 
                   double *t_jacobiana, double *t_sl) {
 
    LIKWID_MARKER_THREADINIT;
    *t_jacobiana = 0.0;
    *t_sl = 0.0;
    int iter = 0;
    
    while (iter < max_iter) {
        avalia_funcao(X, F, n);

        if (iter > 0) fprintf(saida, "#\n");
        for (int i = 0; i < n; ++i) {
            fprintf(saida, "x%d = %.10g\n", i + 1, X[i]);
        }

        // Usa fabs() que adiciona overhead de chamada de função no laço
        real_t max_erro = 0.0;
        for (int i = 0; i < n; ++i) {
            real_t val_abs = fabs(F[i]);
            if (val_abs > max_erro) max_erro = val_abs;
        }
        if (max_erro <= epsilon && epsilon > 0.0) break; 

        LIKWID_MARKER_START("Jacobiana");
        double tempo_j_inicio = timestamp();
        atualiza_jacobiana(X, J, n);
        *t_jacobiana += (timestamp() - tempo_j_inicio);
        LIKWID_MARKER_STOP("Jacobiana");

        LIKWID_MARKER_START("SistemaLinear");
        double tempo_sl_inicio = timestamp();
        
        if (resolve_sistema_linear(J, F, dX, n) != 0) {
            return -1;
        }
        *t_sl += (timestamp() - tempo_sl_inicio);
        LIKWID_MARKER_STOP("SistemaLinear");

        for (int i = 0; i < n; ++i) {
            X[i] += dX[i];
        }
        iter++;
    }
    return 0;
}