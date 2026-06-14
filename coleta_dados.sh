#!/bin/bash

TAMANHOS=(32 64 128 256 512 1000 2000 4000 8000 9000 10000 20000)

X0="1.0"
EPSILON="0.0"
MAX_ITER="25"

OUTPUT_DIR="./resultados_oficiais"
mkdir -p "${OUTPUT_DIR}"

#Nomes dos grupos da CPU AMD Zen
GRUPOS_OFICIAIS="L3 CACHE FLOPS_DP"
CPU=3

echo "=== INICIANDO COLETA PARA OS GRÁFICOS DO ENUNCIADO ==="

for grupo in $GRUPOS_OFICIAIS; do
    echo "=> Coletando métricas do grupo: $grupo"
    
    for N in "${TAMANHOS[@]}"; do
        LOG_FILE="${OUTPUT_DIR}/broyden_${grupo}_N${N}.txt"
        
        echo "$N $X0 $EPSILON $MAX_ITER" | \
        /home/soft/likwid/bin/likwid-perfctr -C ${CPU} -g "$grupo" -m ./broyden > "${LOG_FILE}" 2>&1
    done
done

echo "=== COLETA CONCLUÍDA! ==="
echo "Os arquivos para gerar os gráficos estão salvos em: ${OUTPUT_DIR}"
