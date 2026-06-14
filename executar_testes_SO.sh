#!/bin/bash

# Array com todos os tamanhos de N exigidos pelo enunciado
TAMANHOS=(32 64 128 256 512 1000 2000)

# Parâmetros fixos para os testes de desempenho
X0="1.0"
EPSILON="0.0"
MAX_ITER="25"

# Cria o diretório para salvar as saídas textuais brutas do LIKWID
mkdir -p resultados_likwid

echo "=== Iniciando a coleta de dados com LIKWID (perfctr) ==="

# Loop pelos grupos do LIKWID exigidos pelo professor
for grupo in DATA CACHE FLOPS_DP; do
    echo "Coletando grupo: $grupo"
    
    for N in "${TAMANHOS[@]}"; do
        # Passamos os parâmetros via stdin para o executável ./broyden
        # Executando no core 3 (-C 3) igual ao script de teste do professor
        echo "$N $X0 $EPSILON $MAX_ITER" | \
        /home/soft/likwid/bin/likwid-perfctr -C 3 -g "$grupo" -m ./broyden > "resultados_likwid/broyden_${grupo}_N${N}.txt"
    done
done

echo "=== Testes concluídos! Arquivos gerados em ./resultados_likwid ==="
