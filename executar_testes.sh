#!/bin/bash

# Array com todos os tamanhos de N exigidos pelo enunciado
TAMANHOS=(32 64 128 256 512 1000 2000 4000 8000 9000 10000 20000)

# Parâmetros fixos para os testes de desempenho
X0="1.0"
EPSILON="0.0"
MAX_ITER="25"

# Cria um diretório para salvar as saídas textuais brutas do LIKWID
mkdir -p resultados_likwid

echo "=== Iniciando a coleta de dados com LIKWID ==="

# Loop pelos grupos do LIKWID exigidos pelo professor
for grupo in MEM L2CACHE FLOPS_DP; do
    echo "Coletando grupo: $grupo"
    
    for N in "${TAMANHOS[@]}"; do
        # Executa o LIKWID no núcleo 0 (-C 0) salvando a saída em um arquivo de texto
        # Passamos as variáveis N X0 EPSILON MAX_ITER via echo para o stdin do programa
        echo "$N $X0 $EPSILON $MAX_ITER" | \
        likwid-perfmon -C 0 -g "$grupo" -m ./broyden > "resultados_likwid/broyden_${grupo}_N${N}.txt"
    done
done

echo "=== Testes concluídos! Arquivos gerados em ./resultados_likwid ==="