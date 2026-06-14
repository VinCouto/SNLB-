#!/bin/bash

OUTPUT_DIR="./resultados_escalaridade"
mkdir -p "${OUTPUT_DIR}"
export LIKWID_HOME=/home/soft/likwid


TAMANHOS="1000 2000 4000 8000 16000"

# Vamos focar no grupo CACHE, que é onde a escalaridade quebra fisicamente
GRUPO="CACHE"
CPU=3

echo "=== INICIANDO TESTE DE ESTRESSE DE ESCALARIDADE ==="
echo "Monitorando o comportamento do cache para N grandes no Core ${CPU}..."
echo "------------------------------------------------------------------"

for N in $TAMANHOS
do
    echo "=> Rodando para N = ${N}..."
    
    LOG_FILE="${OUTPUT_DIR}/broyden_${GRUPO}_N${N}.txt"
    
    #Executa passando N, X0, EPSILON e MAX_ITER via stdin igual ao original
    echo "${N} 1.0 0.0 25" | likwid-perfctr -C ${CPU} -g ${GRUPO} -m ./broyden > "${LOG_FILE}" 2>&1
    
    if [ -f "${LOG_FILE}" ]; then
        TIME=$(grep "RDTSC Runtime" "${LOG_FILE}" | head -n 1 | awk -F'|' '{print $3}' | tr -d ' ')
        ACCESS=$(grep "data cache requests" "${LOG_FILE}" | head -n 1 | awk -F'|' '{print $3}' | tr -d ' ')
        CPI=$(grep -w "CPI" "${LOG_FILE}" | head -n 1 | awk -F'|' '{print $3}' | tr -d ' ')
        
        echo "   Tempo total: ${TIME} s"
        echo "   CPI Global:  ${CPI}"
        echo "   Acessos L1:  ${ACCESS}"
        echo "   Log salvo em: ${LOG_FILE}"
    else
        echo "   [ERRO] Falha ao gerar o log para N = ${N}"
    fi
    echo "------------------------------------------------------------------"
done

echo "=== TESTE CONCLUÍDO! ==="
echo "Verifique os logs detalhados na pasta: ${OUTPUT_DIR}"
