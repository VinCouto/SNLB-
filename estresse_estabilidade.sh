#!/bin/bash

OUTPUT_DIR="./resultados_estabilidade"
mkdir -p "${OUTPUT_DIR}"
export LIKWID_HOME=/home/soft/likwid

# Mantemos N fixo para isolar o efeito matemático
N=1000
EPSILON="0.0"

#Cenários de estresse: Pontos iniciais normais e Chutes absurdamente distantes
PONTOS_INICIAIS="1.0 10.0 100.0 1000.0 10000.0"
MAX_ITER=25

GRUPO="FLOPS_DP"  
CPU=3

echo "=== INICIANDO TESTE DE ESTRESSE DE ESTABILIDADE (MAL CONDICIONAMENTO) ==="
echo "Monitorando MFLOP/s para pontos iniciais (X0) agressivos no Core ${CPU}..."
echo "------------------------------------------------------------------"

for X0 in $PONTOS_INICIAIS
do
    echo "=> Rodando com X0 = ${X0}..."
    
    LOG_FILE="${OUTPUT_DIR}/broyden_X0_${X0}.txt"
    
    echo "$N $X0 $EPSILON $MAX_ITER" | \
    likwid-perfctr -C ${CPU} -g ${GRUPO} -m ./broyden > "${LOG_FILE}" 2>&1
    
    if [ -f "${LOG_FILE}" ]; then
        TIME=$(grep "RDTSC Runtime" "${LOG_FILE}" | head -n 1 | awk -F'|' '{print $3}' | tr -d ' ')
        FLOPS=$(grep "DP MFLOP/s" "${LOG_FILE}" | head -n 1 | awk -F'|' '{print $3}' | tr -d ' ')
        
        echo "   Tempo de Execução: ${TIME} s"
        echo "   Performance:       ${FLOPS} MFLOP/s"
        echo "   Log salvo em:      ${LOG_FILE}"
    else
        echo "   [ERRO] Falha ao gerar o log para X0 = ${X0}"
    fi
    echo "------------------------------------------------------------------"
done

echo "=== TESTE CONCLUÍDO! ==="
echo "Verifique se o código divergiu ou estagnou abrindo os logs em: ${OUTPUT_DIR}"
