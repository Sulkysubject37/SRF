#!/usr/bin/env bash
set -e

echo "[SRF] Starting Moderate-Scale Stress Tests..."

CSV_FILE="results/csv/extreme_log.csv"
PLATFORM=$(uname)
mkdir -p results/csv
echo "algorithm,platform,runtime_us,recompute_events,working_set_proxy,dataset_scale,input_size,status" > $CSV_FILE

# Helper to run a test
run_stress() {
    local bin=$1
    local alg=$2
    local arg1=$3
    local arg2=$4
    local arg3=$5
    local scale=$6
    local size=$7
    
    echo "Testing $alg at scale $scale..."
    local output=$(./build/$bin "$arg1" "$arg2" "$arg3" 1 1 "$scale")
    local runtime=$(echo "$output" | grep -w "Time_us:" | cut -d' ' -f2- | tr -d '')
    local recompute=$(echo "$output" | grep -w "Recompute_Events:" | cut -d' ' -f2- | tr -d '')
    local ws=$(echo "$output" | grep -w "Working_Set_Proxy:" | cut -d' ' -f2- | tr -d '')
    
    echo "$alg,$PLATFORM,$runtime,$recompute,$ws,$scale,$size,Success" >> $CSV_FILE
}

# Moderate Scale: 10k
run_stress nw_blocked "Needleman-Wunsch" "datasets_extreme/sequences/seq_a_10000.txt" "datasets_extreme/sequences/seq_b_10000.txt" 20 "MODERATE" 10000
run_stress forward_checkpoint "Forward" "datasets_extreme/sequences/seq_a_10000.txt" 20 10 "MODERATE" 10000
run_stress graph_recompute "Graph-DP" "datasets_extreme/graphs/graph_10000.txt" 4 20 "MODERATE" 10000

echo "[SRF] Moderate-scale tests finished."
