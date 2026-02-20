#!/usr/bin/env bash
set -e

echo "[SRF] Starting EXTREME-Scale Stress Tests..."

CSV_FILE="results/csv/extreme_log.csv"
PLATFORM=$(uname)
# Note: Header already created by moderate_scale.sh or should be created here if not exists
if [ ! -f "$CSV_FILE" ]; then
    mkdir -p results/csv
    echo "algorithm,platform,runtime_us,recompute_events,working_set_proxy,dataset_scale,input_size,status" > $CSV_FILE
fi

run_stress() {
    local bin=$1
    local alg=$2
    local arg1=$3
    local arg2=$4
    local arg3=$5
    local scale=$6
    local size=$7
    
    echo "Testing $alg at scale $size ($scale)..."
    local output=$(./build/$bin "$arg1" "$arg2" "$arg3" 1 1 "$scale")
    local runtime=$(echo "$output" | grep -w "Time_us:" | cut -d' ' -f2- | tr -d '')
    local recompute=$(echo "$output" | grep -w "Recompute_Events:" | cut -d' ' -f2- | tr -d '')
    local ws=$(echo "$output" | grep -w "Working_Set_Proxy:" | cut -d' ' -f2- | tr -d '')
    
    echo "$alg,$PLATFORM,$runtime,$recompute,$ws,$scale,$size,Success" >> $CSV_FILE
}

# 1. Needleman-Wunsch (Feasible upper bound - maybe up to 50k)
run_stress nw_blocked "Needleman-Wunsch" "datasets_extreme/sequences/seq_a_10000.txt" "datasets_extreme/sequences/seq_b_10000.txt" 20 "EXTREME" 10000
run_stress nw_blocked "Needleman-Wunsch" "datasets_extreme/sequences/seq_a_50000.txt" "datasets_extreme/sequences/seq_b_50000.txt" 20 "EXTREME" 50000

# 2. HMM Forward (Full ladder up to 1M)
for size in 10000 50000 100000 500000 1000000; do
    run_stress forward_checkpoint "Forward" "datasets_extreme/sequences/seq_a_$size.txt" 20 10 "EXTREME" $size
done

# 3. Graph-DP (Full ladder up to 250k)
for size in 10000 50000 100000 250000; do
    run_stress graph_recompute "Graph-DP" "datasets_extreme/graphs/graph_$size.txt" 4 20 "EXTREME" $size
done

echo "[SRF] Extreme-scale tests finished."
