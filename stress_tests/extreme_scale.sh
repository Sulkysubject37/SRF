#!/usr/bin/env bash
set -e

echo "[SRF] Starting EXTREME-Scale Stress Tests..."

CSV_FILE="results/csv/extreme_log.csv"
PLATFORM=$(uname)
# Note: Header should be preserved if moderate_scale.sh ran first
if [ ! -f "$CSV_FILE" ]; then
    mkdir -p results/csv
    echo "algorithm,platform,runtime_us,recompute_events,compute_events,memory_access_proxy,working_set_proxy,dataset_scale,input_size,status" > $CSV_FILE
fi

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
    local output=$(./build/$bin "$arg1" "$arg2" "$arg3" "$arg4" "$arg5")
    
    local runtime=$(echo "$output" | grep -w "Time_us:" | cut -d' ' -f2- | tr -d '\r')
    local recompute=$(echo "$output" | grep -w "Recompute_Events:" | cut -d' ' -f2- | tr -d '\r')
    local compute=$(echo "$output" | grep -w "Compute_Events:" | cut -d' ' -f2- | tr -d '\r')
    local mem_proxy=$(echo "$output" | grep -w "Memory_Access_Proxy:" | cut -d' ' -f2- | tr -d '\r')
    local ws=$(echo "$output" | grep -w "Working_Set_Proxy:" | cut -d' ' -f2- | tr -d '\r')
    
    echo "$alg,$PLATFORM,$runtime,$recompute,$compute,$mem_proxy,$ws,$scale_label,$size,Success" >> $CSV_FILE
}

# 1. Needleman-Wunsch (Quadratic - 50k limit for local run)
for size in 10000 50000; do
    run_stress nw_blocked "Needleman-Wunsch" "datasets_extreme/sequences/seq_a_$size.txt" "datasets_extreme/sequences/seq_b_$size.txt" 20 1 "EXTREME" "EXTREME" $size
done

# 2. HMM Forward (Linear ladder up to 1M)
for size in 10000 50000 100000 500000 1000000; do
    run_stress forward_checkpoint "Forward" "datasets_extreme/sequences/seq_a_$size.txt" 20 1 "EXTREME" "" "EXTREME" $size
done

# 3. Graph-DP (Log-Linear ladder up to 250k)
for size in 10000 50000 100000 250000; do
    run_stress graph_recompute "Graph-DP" "datasets_extreme/graphs/graph_$size.txt" 4 1 "EXTREME" "" "EXTREME" $size
done

echo "[SRF] Extreme-scale tests finished."