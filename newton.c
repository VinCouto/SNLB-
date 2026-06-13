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
    
    // Inicializa os acumuladores de tempo passados pelo main
    *t_jacobiana = 0.0;
    *t_sl = 0.0;

    int iter = 0;
    
    while (iter < max_iter) {
        // 1. Avalia a função não-linear F(X) no ponto atual X
        avalia_funcao(X, F, n);

        // 2. IMPRESSÃO DAS INCÓGNITAS DA ITERAÇÃO ATUAL (Formato exigido pelo enunciado)
        // Se for a iteração 0, imprime direto; se for depois, separa por '#'
        if (iter > 0) {
            fprintf(saida, "#\n");
        }
        for (int i = 0; i < n; ++i) {
            fprintf(saida, "x%d = %.10g\n", i + 1, X[i]);
        }

        // 3. CRITÉRIO DE PARADA: Verifica se a norma infinita (ou o maior elemento) de F(X) é < epsilon
        // Como o enunciado pede epsilon = 0 para os testes de 25 iterações, isto serve para o caso geral
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

        // 4. CÁLCULO DA MATRIZ JACOBIANA (INSTRUMENTADO)
        LIKWID_MARKER_START("Jacobiana");
        double tempo_j_inicio = timestamp();
        
        atualiza_diagonal_jacobiana(X, diag_principal, n);
        
        *t_jacobiana += (timestamp() - tempo_j_inicio);
        LIKWID_MARKER_STOP("Jacobiana");

        // 5. RESOLUÇÃO DO SISTEMA LINEAR: J * dX = -F(X) (INSTRUMENTADO)
        // Lembrando: resolve_sistema_tridiagonal altera o vetor F in-place, que vira dX
        LIKWID_MARKER_START("SistemaLinear");
        double tempo_sl_inicio = timestamp();
        
        if (resolve_sistema_tridiagonal(diag_principal, F, n) != 0) {
            fprintf(stderr, "Erro: Matriz Jacobiana tornou-se singular na iteração %d.\n", iter);
            return -1;
        }
        
        *t_sl += (timestamp() - tempo_sl_inicio);
        LIKWID_MARKER_STOP("SistemaLinear");

        // 6. ATUALIZAÇÃO DO VETOR X: X^(k+1) = X^(k) + dX
        // Como F agora guarda dX, fazemos X[i] += F[i]
        // Loop simples perfeitamente vetorizável com AVX
        for (int i = 0; i < n; ++i) {
            X[i] += F[i];
        }

        iter++;
    }

    return 0;
}