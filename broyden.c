#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <likwid.h>
#include "utils.h"
#include "jacobiana.h"
#include "sistema_linear.h"
#include "newton.h"

int main(int argc, char *argv[]) {
    LIKWID_MARKER_INIT;

    int n = 0;
    real_t x0 = 0.0;
    real_t epsilon = 0.0;
    int max_iter = 0;
    char *arquivo_saida = NULL;

    int opt;
    while ((opt = getopt(argc, argv, "o:")) != -1) {
        if (opt == 'o') arquivo_saida = optarg;
    }

    if (scanf("%d %lf %lf %d", &n, &x0, &epsilon, &max_iter) != 4) return 1;

    FILE *saida = stdout;
    if (arquivo_saida != NULL) {
        saida = fopen(arquivo_saida, "w");
    }

    // ALOCAÇÃO INGÉNUA COM MALLOC (Gera Cache Misses e impede vetorização AVX perfeita)
    real_t *X = (real_t *)malloc(n * sizeof(real_t));
    real_t *F = (real_t *)malloc(n * sizeof(real_t));
    real_t *dX = (real_t *)malloc(n * sizeof(real_t));
    
    // Aloca matriz completa N x N para a Jacobiana
    real_t **J = (real_t **)malloc(n * sizeof(real_t *));
    for (int i = 0; i < n; i++) {
        J[i] = (real_t *)malloc(n * sizeof(real_t));
        X[i] = x0;
    }   

    double t_total = 0.0, t_jacobiana = 0.0, t_sl = 0.0;

    LIKWID_MARKER_START("NewtonTotal");
    t_total = timestamp();

    int status = executa_newton(n, X, F, dX, J, epsilon, max_iter, saida, &t_jacobiana, &t_sl);
    
    t_total = timestamp() - t_total;
    LIKWID_MARKER_STOP("NewtonTotal");

    fprintf(saida, "###########\n");
    fprintf(saida, "# Tempo Total: %.10g\n", t_total);
    fprintf(saida, "# Tempo Jacobiana: %.10g\n", t_jacobiana);
    fprintf(saida, "# Tempo SL: %.10g\n", t_sl);
    fprintf(saida, "###########\n");

    for (int i = 0; i < n; i++) free(J[i]);
    free(J);
    free(X); free(F); free(dX);
    if (arquivo_saida != NULL) fclose(saida);

    LIKWID_MARKER_CLOSE;
    return 0;
}