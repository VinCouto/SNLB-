#include <stdio.h>
#include <stdlib.h>
#include "sistema_linear.h"
#include "utils.h"

int resolve_sistema_tridiagonal(const real_t *restrict diag_principal, real_t *restrict F, int n) {
    if (n <= 0) return -1;

    //Se o sistema tiver tamanho 1, resolve diretamente
    if (n == 1) {
        if (diag_principal[0] == 0.0) return -1; // Matriz singular
        F[0] = -F[0] / diag_principal[0];
        return 0;
    }

    /* OTIMIZAÇÃO DE MEMÓRIA:
       O algoritmo de Thomas tradicional modifica a superdiagonal (b) e o vetor de termos independentes (d).
       Como nossa superdiagonal original é constante (-2.0), precisamos apenas de um vetor auxiliar 
       temporário de tamanho N para guardar os coeficientes modificados da superdiagonal (c_prime).
       Usamos alocação alinhada para máxima performance.
    */
    real_t *c_prime;
    if (posix_memalign((void**)&c_prime, 64, n * sizeof(real_t)) != 0) {
        return -1;
    }

    //Inversão do vetor F original in-place: o método resolve J * dX = -F
    for (int i = 0; i < n; ++i) {
        F[i] = -F[i];
    }

    
    //Linha 0
    //Coeficiente superdiagonal original é -2.0
    real_t d_atual = diag_principal[0];
    if (d_atual == 0.0) {
        free(c_prime);
        return -1; 
    }
    
    c_prime[0] = -2.0 / d_atual;
    F[0] = F[0] / d_atual;

    //Caso padrão
    //Coeficientes originais fixos: subdiagonal (a) = -1.0, superdiagonal (b) = -2.0
    for (int i = 1; i < n - 1; ++i) {

        //d_atual = principal[i] - (a * c_prime[i-1]) -> Como a = -1.0: principal[i] + c_prime[i-1]
        d_atual = diag_principal[i] + c_prime[i - 1];
        
        if (d_atual == 0.0) {
            free(c_prime);
            return -1;
        }

        c_prime[i] = -2.0 / d_atual;
        // F[i] = (F[i] - a * F[i-1]) / d_atual -> Como a = -1.0: (F[i] + F[i-1]) / d_atual
        F[i] = (F[i] + F[i - 1]) / d_atual;
    }

    //Último caso (n-1) - Não possui superdiagonal, apenas atualiza d_atual e F
    d_atual = diag_principal[n - 1] + c_prime[n - 2];
    if (d_atual == 0.0) {
        free(c_prime);
        return -1;
    }
    F[n - 1] = (F[n - 1] + F[n - 2]) / d_atual;

    //Vetor F agora começa a receber os valores de dX (solução real) de trás para a frente
    for (int i = n - 2; i >= 0; --i) {
        F[i] = F[i] - c_prime[i] * F[i + 1];
    }

    free(c_prime);
    return 0;
}