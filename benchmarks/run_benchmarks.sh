#!/usr/bin/env bash
set -e

echo "[SRF] Starting Fixed-Memory Runtime Benchmarks (Phase 3)..."

# Ensure we are in the root directory
cd "$(dirname "$0")/.."

# Build all binaries
bash benchmarks/build.sh

# CSV Schema: algorithm,variant,platform,runtime_us,peak_memory_kb,recompute_events,cache_budget,working_set_proxy,locality_proxy,param_1,param_2,param_3,status
CSV_FILE="results/csv/benchmark_log.csv"
PLATFORM=$(uname)

mkdir -p results/csv
echo "algorithm,variant,platform,runtime_us,peak_memory_kb,recompute_events,cache_budget,working_set_proxy,locality_proxy,param_1,param_2,param_3,status" > $CSV_FILE

# Helper to log a run
log_run() {
    local alg=$1
    local var=$2
    local bin=$3
    local p1=$4 # TileSize / K-Interval / Depth
    local p2=$5 # SeqLen / NumNodes
    local p3=$6 # CacheBudget / LocalityMode / ScheduleMode
    
    echo "Running $alg ($var) P1=$p1 P2=$p2 P3=$p3..."
    local output=$(./build/$bin "$p2" "$p1" "$p3")
    
    local runtime=$(echo "$output" | grep "Time_us:" | cut -d' ' -f2-)
    local memory=$(echo "$output" | grep "Memory_kb:" | cut -d' ' -f2-)
    local recompute=$(echo "$output" | grep "Recompute_Events:" | cut -d' ' -f2-)
    local cache_budget=$(echo "$output" | grep "Param_3:" | cut -d' ' -f2-)
    local working_set=$(echo "$output" | grep "Working_Set_Proxy:" | cut -d' ' -f2-)
    local locality=$(echo "$output" | grep "Locality_Proxy:" | cut -d' ' -f2-)
    
    [ -z "$recompute" ] && recompute=0
    [ -z "$working_set" ] && working_set=0
    [ -z "$locality" ] && locality=0
    [ -z "$cache_budget" ] && cache_budget=0
    
    echo "$alg,$var,$PLATFORM,$runtime,$memory,$recompute,$cache_budget,$working_set,$locality,$p1,$p2,$p3,Success" >> $CSV_FILE
}

# 1. Needleman-Wunsch Sweep (Cache Budget)
for seq in 400; do
    log_run "Needleman-Wunsch" "Baseline" "needleman_wunsch" 0 "$seq" 0
    for budget in 1 4 16 64; do
        log_run "Needleman-Wunsch" "SRF-CacheAware" "nw_blocked" 20 "$seq" "$budget"
    done
done

# 2. Forward/Viterbi Sweep (Locality Mode)
for seq in 1000; do
    for alg in "Forward" "Viterbi"; do
        bin_name=$(echo "$alg" | tr '[:upper:]' '[:lower:]')
        log_run "$alg" "Baseline" "$bin_name" 0 "$seq" 0
        for mode in 0 1; do
            log_run "$alg" "SRF-LocalityAware" "${bin_name}_checkpoint" 20 "$seq" "$mode"
        done
    done
done

# 3. Graph-DP Sweep (Schedule Mode)
for nodes in 2000; do
    log_run "Graph-DP" "Baseline" "graph_dp" 0 "$nodes" 0
    for mode in 0 1; do
        log_run "Graph-DP" "SRF-LocalityAware" "graph_recompute" 4 "$nodes" "$mode"
    done
done

echo "[SRF] Generating Phase 3 plots..."
# Important: ensure correct python/venv use if needed
python3 benchmarks/generate_plots.py

echo "[SRF] Benchmarks finished."