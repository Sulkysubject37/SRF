#!/usr/bin/env bash
set -e

echo "[SRF] Starting Moderate-Scale Stress Tests..."

CSV_FILE="results/csv/extreme_log.csv"
PLATFORM=$(uname)
mkdir -p results/csv
echo "algorithm,platform,runtime_us,recompute_events,compute_events,memory_access_proxy,working_set_proxy,dataset_scale,input_size,status" > $CSV_FILE

# Helper to run a test
run_stress() {
    local bin=$1
    local alg=$2
    local arg1=$3 # PathA / Path
    local arg2=$4 # PathB / K / Depth
    local arg3=$5 # B / G / G
    local arg4=$6 # G / Scale / Scale
    local arg5=$7 # Scale / "" / ""
    local scale_label=$8
    local size=$9
    
    echo "Testing $alg at scale $size ($scale_label)..."
    # Mapping to binary: [arg1] [arg2] [arg3] [arg4] [arg5]
    local output=$(./build/$bin "$arg1" "$arg2" "$arg3" "$arg4" "$arg5")
    
    local runtime=$(echo "$output" | grep -w "Time_us:" | cut -d' ' -f2- | tr -d '\r')
    local recompute=$(echo "$output" | grep -w "Recompute_Events:" | cut -d' ' -f2- | tr -d '\r')
    local compute=$(echo "$output" | grep -w "Compute_Events:" | cut -d' ' -f2- | tr -d '\r')
    local mem_proxy=$(echo "$output" | grep -w "Memory_Access_Proxy:" | cut -d' ' -f2- | tr -d '\r')
    local ws=$(echo "$output" | grep -w "Working_Set_Proxy:" | cut -d' ' -f2- | tr -d '\r')
    
    echo "$alg,$PLATFORM,$runtime,$recompute,$compute,$mem_proxy,$ws,$scale_label,$size,Success" >> $CSV_FILE
}

# Moderate Scale: 10k
# NW: [PathA] [PathB] [B] [G] [Scale]
run_stress nw_blocked "Needleman-Wunsch" "datasets_extreme/sequences/seq_a_10000.txt" "datasets_extreme/sequences/seq_b_10000.txt" 20 1 "MODERATE" "MODERATE" 10000
# Forward: [Path] [K] [G] [Scale]
run_stress forward_checkpoint "Forward" "datasets_extreme/sequences/seq_a_10000.txt" 20 1 "MODERATE" "" "MODERATE" 10000
# Graph: [Path] [Depth] [G] [Scale]
run_stress graph_recompute "Graph-DP" "datasets_extreme/graphs/graph_10000.txt" 4 1 "MODERATE" "" "MODERATE" 10000

echo "[SRF] Moderate-scale tests finished."