#!/bin/bash

TAMANHOS=(32 64 128 256 512 1000 2000 4000 8000 9000 10000 20000)

X0="1.0"
EPSILON="0.0"
MAX_ITER="25"

mkdir -p resultados_likwid

echo "=== Iniciando a coleta de dados com LIKWID (perfctr) ==="

for grupo in DATA CACHE FLOPS_DP; do
    echo "Coletando grupo: $grupo"
    
    for N in "${TAMANHOS[@]}"; do
        echo "$N $X0 $EPSILON $MAX_ITER" | \
        /home/soft/likwid/bin/likwid-perfctr -C 3 -g "$grupo" -m ./broyden > "resultados_likwid/broyden_${grupo}_N${N}.txt"
    done
done

echo "=== Testes concluídos! Arquivos gerados em ./resultados_likwid ==="
