#include <stdio.h>
#include <stdlib.h>
#include <likwid.h>
#include "newton.h"
#include "jacobiana.h"
#include "sistema_linear.h"
#include "utils.h"

int executa_newton(int n, real_t *restrict X, real_t *restrict F, real_t *restrict diag_principal,
                   real_t epsilon, int max_iter, FILE *saida, 
                   double *t_jacobiana, double *t_sl) {
 
    LIKWID_MARKER_THREADINIT;

    //Inicializa os acumuladores de tempo passados pelo main
    *t_jacobiana = 0.0;
    *t_sl = 0.0;

    int iter = 0;
    
    while (iter < max_iter) {

        avalia_funcao(X, F, n);

        //Se for a iteração 0, imprime direto; se for depois, separa por '#'
        if (iter > 0) {
            fprintf(saida, "#\n");
        }
        for (int i = 0; i < n; ++i) {
            fprintf(saida, "x%d = %.10g\n", i + 1, X[i]);
        }

        //Verifica se a norma infinita de F(X) é < epsilon
        real_t max_erro = 0.0;
        for (int i = 0; i < n; ++i) {
            real_t val_abs = ABS(F[i]);
            if (val_abs > max_erro) {
                max_erro = val_abs;
            }
        }
        if (max_erro <= epsilon && epsilon > 0.0) {
            break; 
        }

        //Calculo da Jacobiana
        LIKWID_MARKER_START("Jacobiana");
        double tempo_j_inicio = timestamp();
        
        atualiza_diagonal_jacobiana(X, diag_principal, n);
        
        *t_jacobiana += (timestamp() - tempo_j_inicio);
        LIKWID_MARKER_STOP("Jacobiana");

        //Resolução do sistema linear J * dX = -F(X) (
        //Altera o vetor F in-place, que vira dX
        LIKWID_MARKER_START("SistemaLinear");
        double tempo_sl_inicio = timestamp();
        
        if (resolve_sistema_tridiagonal(diag_principal, F, n) != 0) {
            fprintf(stderr, "Erro: Matriz Jacobiana tornou-se singular na iteração %d.\n", iter);
            return -1;
        }
        
        *t_sl += (timestamp() - tempo_sl_inicio);
        LIKWID_MARKER_STOP("SistemaLinear");

        //Atualização do vetor X^(k+1) = X^(k) + dX
        //F agora guarda dX, fazemos X[i] += F[i]
        for (int i = 0; i < n; ++i) {
            X[i] += F[i];
        }

        iter++;
    }

    return 0;
}
