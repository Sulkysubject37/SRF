#!/usr/bin/env bash
set -e

echo "[SRF] Starting Parametric Benchmarks..."

# Ensure we are in the root directory
cd "$(dirname "$0")/.."

# Build all binaries
bash benchmarks/build.sh

# CSV Schema: algorithm,variant,platform,runtime_us,peak_memory_kb,recompute_events,parameter_1,parameter_2,notes
CSV_FILE="results/csv/benchmark_log.csv"
PLATFORM=$(uname)

mkdir -p results/csv
echo "algorithm,variant,platform,runtime_us,peak_memory_kb,recompute_events,parameter_1,parameter_2,notes" > $CSV_FILE

# Helper to log a run
log_run() {
    local alg=$1
    local var=$2
    local bin=$3
    local p1=$4
    local p2=$5
    
    echo "Running $alg ($var) P1=$p1 P2=$p2..."
    local output=$(./build/$bin "$p2" "$p1") # Note: p2=seq_len, p1=param
    
    local runtime=$(echo "$output" | grep "Time_us:" | cut -d' ' -f2-)
    local memory=$(echo "$output" | grep "Memory_kb:" | cut -d' ' -f2-)
    local recompute=$(echo "$output" | grep "Recompute_Events:" | cut -d' ' -f2-)
    [ -z "$recompute" ] && recompute=0
    
    echo "$alg,$var,$PLATFORM,$runtime,$memory,$recompute,$p1,$p2,Success" >> $CSV_FILE
}

# 1. Needleman-Wunsch Sweep
for seq in 200 400; do
    # Baseline
    log_run "Needleman-Wunsch" "Baseline" "needleman_wunsch" 0 "$seq"
    # SRF Sweep Tile Sizes
    for tile in 10 20 40; do
        log_run "Needleman-Wunsch" "SRF-Blocked" "nw_blocked" "$tile" "$seq"
    done
done

# 2. Forward/Viterbi Sweep
for seq in 500 1000; do
    # Forward
    log_run "Forward" "Baseline" "forward" 0 "$seq"
    for k in 5 10 20; do
        log_run "Forward" "SRF-Checkpoint" "forward_checkpoint" "$k" "$seq"
    done
    
    # Viterbi
    log_run "Viterbi" "Baseline" "viterbi" 0 "$seq"
    for k in 5 10 20; do
        log_run "Viterbi" "SRF-Checkpoint" "viterbi_checkpoint" "$k" "$seq"
    done
done

# 3. Graph-DP Sweep
for nodes in 1000 2000; do
    # Baseline
    log_run "Graph-DP" "Baseline" "graph_dp" 0 "$nodes"
    # SRF Sweep Recompute Depth
    for depth in 2 4 8; do
        log_run "Graph-DP" "SRF-Recompute" "graph_recompute" "$depth" "$nodes"
    done
done

echo "[SRF] Generating parametric plots..."
python3 benchmarks/generate_plots.py

echo "[SRF] Benchmarks finished."
