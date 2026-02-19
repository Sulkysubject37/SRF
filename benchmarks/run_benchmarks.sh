#!/usr/bin/env bash
set -e

echo "[SRF] Starting Phase 5-A Granularity Experiments..."

# Ensure we are in the root directory
cd "$(dirname "$0")/.."

# Build all binaries
bash benchmarks/build.sh

# CSV Schema Extension for Phase 5-A
CSV_FILE="results/csv/benchmark_log.csv"
PLATFORM=$(uname)

mkdir -p results/csv
echo "algorithm,variant,platform,runtime_us,peak_memory_kb,recompute_events,cache_budget,working_set_proxy,locality_proxy,param_1,param_2,param_3,backend_type,device_memory_budget,transfer_overhead_us,kernel_launch_count,granularity_type,granularity_unit_size,unit_recompute_events,unit_reuse_proxy,status" > $CSV_FILE

# Helper to log a run
log_run() {
    local alg=$1
    local var=$2
    local bin=$3
    local p1=$4 # TileSize / K-Interval / Depth
    local p2=$5 # SeqLen / NumNodes
    local p3=$6 # Granularity UnitSize (G)
    local backend_env=$7 # SRF_FORCE_GPU=1 etc.
    local gpu_budget=${8:-1024}
    
    echo "Running $alg ($var) P1=$p1 P2=$p2 G=$p3 Backend=$backend_env..."
    local output=$(env $backend_env ./build/$bin "$p2" "$p1" "$p3" "$gpu_budget")
    
    # Use -w to avoid matching Unit_Recompute_Events when searching for Recompute_Events
    local runtime=$(echo "$output" | grep -w "Time_us:" | cut -d' ' -f2- | tr -d '\r')
    local memory=$(echo "$output" | grep -w "Memory_kb:" | cut -d' ' -f2- | tr -d '\r')
    local recompute=$(echo "$output" | grep -w "Recompute_Events:" | cut -d' ' -f2- | tr -d '\r')
    local unit_recompute=$(echo "$output" | grep -w "Unit_Recompute_Events:" | cut -d' ' -f2- | tr -d '\r')
    local unit_reuse=$(echo "$output" | grep -w "Unit_Reuse_Proxy:" | cut -d' ' -f2- | tr -d '\r')
    local unit_size=$(echo "$output" | grep -w "Granularity_Unit_Size:" | cut -d' ' -f2- | tr -d '\r')
    
    local working_set=$(echo "$output" | grep -w "Working_Set_Proxy:" | cut -d' ' -f2- | tr -d '\r')
    local backend_type=$(echo "$output" | grep -w "Backend:" | cut -d' ' -f2- | tr -d '\r')
    local transfer=$(echo "$output" | grep -w "Transfer_Overhead_us:" | cut -d' ' -f2- | tr -d '\r')
    local kernels=$(echo "$output" | grep -w "Kernel_Launch_Count:" | cut -d' ' -f2- | tr -d '\r')
    local device_budget=$(echo "$output" | grep -w "Device_Memory_Budget_kb:" | cut -d' ' -f2- | tr -d '\r')

    [ -z "$recompute" ] && recompute=0
    [ -z "$unit_recompute" ] && unit_recompute=0
    [ -z "$unit_reuse" ] && unit_reuse=0
    [ -z "$unit_size" ] && unit_size="NA"
    [ -z "$working_set" ] && working_set=0
    [ -z "$backend_type" ] && backend_type="cpu"
    [ -z "$transfer" ] && transfer="NA"
    [ -z "$kernels" ] && kernels="NA"
    [ -z "$device_budget" ] && device_budget="NA"
    
    # Granularity Type Mapping
    local g_type="unknown"
    [[ "$alg" == "Needleman-Wunsch" ]] && g_type="tile"
    [[ "$alg" == "Forward" || "$alg" == "Viterbi" ]] && g_type="segment"
    [[ "$alg" == "Graph-DP" ]] && g_type="group"

    echo "$alg,$var,$PLATFORM,$runtime,$memory,$recompute,0,$working_set,0,$p1,$p2,$p3,$backend_type,$device_budget,$transfer,$kernels,$g_type,$unit_size,$unit_recompute,$unit_reuse,Success" >> $CSV_FILE
}

# Granularity Sweeps (including G=1 as non-granular baseline)
for backend in "SRF_FORCE_CPU=1" "SRF_FORCE_GPU=1"; do
    # 1. Needleman-Wunsch: Tile Granularity Sweep (G=1, 10, 20, 40)
    for g_size in 1 10 20 40; do
        log_run "Needleman-Wunsch" "SRF-GranularityAware" "nw_blocked" 20 600 "$g_size" "$backend"
    done

    # 2. HMM Forward: Segment Granularity Sweep (G=1, 2, 10, 50)
    for g_size in 1 2 10 50; do
        log_run "Forward" "SRF-GranularityAware" "forward_checkpoint" 20 1000 "$g_size" "$backend"
    done

    # 3. Graph-DP: Group Granularity Sweep (G=1, 5, 20, 100)
    for g_size in 1 5 20 100; do
        log_run "Graph-DP" "SRF-GranularityAware" "graph_recompute" 4 2000 "$g_size" "$backend"
    done
done

echo "[SRF] Generating Phase 5-A plots..."
python3 benchmarks/generate_plots.py

echo "[SRF] Benchmarks finished."