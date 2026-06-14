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

    //Tratamento de argumentos
    int opt;
    while ((opt = getopt(argc, argv, "o:")) != -1) {
        switch (opt) {
            case 'o':
                arquivo_saida = optarg;
                break;
            default:
                fprintf(stderr, "Uso: %s [-o arquivo_saida]\n", argv[0]);
                return 1;
        }
    }

    if (scanf("%d %lf %lf %d", &n, &x0, &epsilon, &max_iter) != 4) {
        fprintf(stderr, "Erro ao ler os parâmetros de entrada (N, x0, epsilon, MAX).\n");
        return 1;
    }

    // Redireciona a saída se o usuário passou o argumento "-o"
    FILE *saida = stdout;
    if (arquivo_saida != NULL) {
        saida = fopen(arquivo_saida, "w");
        if (!saida) {
            fprintf(stderr, "Erro ao abrir o arquivo de saída: %s\n", arquivo_saida);
            return 1;
        }
    }


    //Alocação de memoria alinhada para os vetores usados no método de Newton.
    //Usamos posix_memalign para garantir alinhamento de 64 bytes
    real_t *X, *F, *diag_principal;
    
    if (posix_memalign((void**)&X, 64, n * sizeof(real_t)) != 0 ||
        posix_memalign((void**)&F, 64, n * sizeof(real_t)) != 0 ||
        posix_memalign((void**)&diag_principal, 64, n * sizeof(real_t)) != 0) {
        fprintf(stderr, "Erro catastrófico de alocação de memória alinhada.\n");
        return 1;
    }

    // Inicializa o vetor X com o palpite inicial x0 exigido
    for (int i = 0; i < n; i++) {
        X[i] = x0;
    }   

    // Variáveis para contagem de tempo
    double t_total = 0.0;
    double t_jacobiana = 0.0;
    double t_sl = 0.0;

    // Adicionamos o marcador do LIKWID para o bloco inteiro do Newton
    LIKWID_MARKER_START("NewtonTotal");
    t_total = timestamp();

    //Passamos os ponteiros de tempo para acumular o gasto interno com a 
    //Jacobiana e com o Solucionador Linear
    int status = executa_newton(n, X, F, diag_principal, epsilon, max_iter, saida, &t_jacobiana, &t_sl);
    
    if (status != 0) {
        fprintf(stderr, "Aviso: O método de Newton falhou ou divergiu.\n");
    }
    
    t_total = timestamp() - t_total;
    LIKWID_MARKER_STOP("NewtonTotal");

    //Impressão
    fprintf(saida, "###########\n");
    fprintf(saida, "# Tempo Total: %.10g\n", t_total);
    fprintf(saida, "# Tempo Jacobiana: %.10g\n", t_jacobiana);
    fprintf(saida, "# Tempo SL: %.10g\n", t_sl);
    fprintf(saida, "###########\n");

    //Liberação de recursos
    free(X);
    free(F);
    free(diag_principal);
    if (arquivo_saida != NULL) {
        fclose(saida);
    }

    LIKWID_MARKER_CLOSE;
    return 0;
}