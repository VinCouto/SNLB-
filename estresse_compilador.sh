#!/bin/bash

OUTPUT_DIR="./resultados_compilador"
mkdir -p "${OUTPUT_DIR}"
export LIKWID_HOME=/home/soft/likwid

#Comparando tamanhos perfeitos e tamanhos que quebram a vetorização
TAMANHOS="1000 1001 2000 2003"

X0="1.0"
EPSILON="0.0"
MAX_ITER=25

GRUPO="FLOPS_DP"
CPU=3

echo "=== INICIANDO TESTE DE ESTRESSE DE COMPILADOR ==="
echo "Analisando impacto de N ímpares/primos na vetorização AVX..."
echo "------------------------------------------------------------------"

for N in $TAMANHOS
do
    echo "=> Rodando para N = ${N}..."
    
    LOG_FILE="${OUTPUT_DIR}/broyden_compilador_N${N}.txt"
    
    echo "$N $X0 $EPSILON $MAX_ITER" | \
    likwid-perfctr -C ${CPU} -g ${GRUPO} -m ./broyden > "${LOG_FILE}" 2>&1
    
    if [ -f "${LOG_FILE}" ]; then
        TIME=$(grep "RDTSC Runtime" "${LOG_FILE}" | head -n 1 | awk -F'|' '{print $3}' | tr -d ' ')

        AVX_FLOPS=$(grep "DP MFLOP/s (AVX assumed)" "${LOG_FILE}" | head -n 1 | awk -F'|' '{print $3}' | tr -d ' ')
        
        echo "   Tempo total: ${TIME} s"
        echo "   AVX MFLOP/s: ${AVX_FLOPS}"
        echo "   Log salvo em: ${LOG_FILE}"
    else
        echo "   [ERRO] Falha ao gerar o log para N = ${N}"
    fi
    echo "------------------------------------------------------------------"
done

echo "=== TESTE CONCLUÍDO! ==="
echo "Compare os arquivos na pasta: ${OUTPUT_DIR}"
