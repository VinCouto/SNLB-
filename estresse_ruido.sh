#!/bin/bash

OUTPUT_DIR="./resultados_ruido"
mkdir -p "${OUTPUT_DIR}"
export LIKWID_HOME=/home/soft/likwid

# Usaremos N=4000 que já tem um tamanho considerável para sentir o cache
N=4000
X0="1.0"
EPSILON="0.0"
MAX_ITER=25
GRUPO="CACHE"
CPU=3

echo "=== INICIANDO TESTE DE ESTRESSE DE RUÍDO DE BORDA ==="
echo "------------------------------------------------------------------"

# --- PASSO 1: EXECUÇÃO EM AMBIENTE LIMPO ---
echo "=> 1. Rodando Broyden em ambiente ISOLADO (Core 3)..."
LOG_LIMPO="${OUTPUT_DIR}/broyden_isolado.txt"
echo "$N $X0 $EPSILON $MAX_ITER" | \
likwid-perfctr -C ${CPU} -g ${GRUPO} -m ./broyden > "${LOG_LIMPO}" 2>&1

TIME_LIMPO=$(grep "RDTSC Runtime" "${LOG_LIMPO}" | head -n 1 | awk -F'|' '{print $3}' | tr -d ' ')
MISS_LIMPO=$(grep "DATA_CACHE_REFILLS_ALL" "${LOG_LIMPO}" | head -n 1 | awk -F'|' '{print $3}' | tr -d ' ')

echo "   [Isolado] Tempo: ${TIME_LIMPO} s | Cache Misses (Refills): ${MISS_LIMPO}"
echo "------------------------------------------------------------------"

# --- PASSO 2: GERAR RUÍDO EM BACKGROUND ---
echo "=> 2. Ativando processos intrusos nos cores vizinhos (Ruído)..."
# Dispara loops infinitos nos Cores 1, 2 e 4 (vizinhos de CCX do Core 3)
# Eles vão ficar lendo e escrevendo dados aleatórios para estressar o cache L3
taskset -c 1 dd if=/dev/urandom of=/dev/null bs=1M status=none & PID_RUIDO1=$!
taskset -c 2 dd if=/dev/urandom of=/dev/null bs=1M status=none & PID_RUIDO2=$!
taskset -c 4 dd if=/dev/urandom of=/dev/null bs=1M status=none & PID_RUIDO3=$!

# Aguarda 2 segundos para o ruído estabilizar no processador
sleep 2

# --- PASSO 3: EXECUÇÃO SOB ESTRESSE ---
echo "=> 3. Rodando Broyden sob a tempestade de RUÍDO (Core 3)..."
LOG_RUIDO="${OUTPUT_DIR}/broyden_com_ruido.txt"
echo "$N $X0 $EPSILON $MAX_ITER" | \
likwid-perfctr -C ${CPU} -g ${GRUPO} -m ./broyden > "${LOG_RUIDO}" 2>&1

TIME_RUIDO=$(grep "RDTSC Runtime" "${LOG_RUIDO}" | head -n 1 | awk -F'|' '{print $3}' | tr -d ' ')
MISS_RUIDO=$(grep "DATA_CACHE_REFILLS_ALL" "${LOG_RUIDO}" | head -n 1 | awk -F'|' '{print $3}' | tr -d ' ')

echo "   [Com Ruído] Tempo: ${TIME_RUIDO} s | Cache Misses (Refills): ${MISS_RUIDO}"
echo "------------------------------------------------------------------"

# --- PASSO 4: LIMPEZA ---
echo "=> 4. Finalizando os processos intrusos..."
kill $PID_RUIDO1 $PID_RUIDO2 $PID_RUIDO3
wait $PID_RUIDO1 $PID_RUIDO2 $PID_RUIDO3 2>/dev/null

echo "=== TESTE CONCLUÍDO! ==="
