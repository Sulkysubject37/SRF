#!/usr/bin/env bash
set -e

echo "[SRF] Starting Phase 4 Scalability & Multi-Backend Benchmarks..."

# Ensure we are in the root directory
cd "$(dirname "$0")/.."

# Build all binaries
bash benchmarks/build.sh

# CSV Schema Extension for Phase 4
# algorithm,variant,platform,runtime_us,peak_memory_kb,recompute_events,cache_budget,working_set_proxy,locality_proxy,param_1,param_2,param_3,backend_type,device_memory_budget,transfer_overhead_us,kernel_launch_count,status
CSV_FILE="results/csv/benchmark_log.csv"
PLATFORM=$(uname)

mkdir -p results/csv
echo "algorithm,variant,platform,runtime_us,peak_memory_kb,recompute_events,cache_budget,working_set_proxy,locality_proxy,param_1,param_2,param_3,backend_type,device_memory_budget,transfer_overhead_us,kernel_launch_count,status" > $CSV_FILE

# Helper to log a run
log_run() {
    local alg=$1
    local var=$2
    local bin=$3
    local p1=$4 # TileSize / K-Interval / Depth
    local p2=$5 # SeqLen / NumNodes
    local p3=$6 # CacheBudget / LocalityMode / ScheduleMode
    local backend_env=$7 # SRF_FORCE_GPU=1 etc.
    local gpu_budget=${8:-1024}
    
    echo "Running $alg ($var) P1=$p1 P2=$p2 P3=$p3 Backend=$backend_env..."
    local output=$(env $backend_env ./build/$bin "$p2" "$p1" "$p3" "$gpu_budget")
    
    local runtime=$(echo "$output" | grep "Time_us:" | cut -d' ' -f2-)
    local memory=$(echo "$output" | grep "Memory_kb:" | cut -d' ' -f2-)
    local recompute=$(echo "$output" | grep "Recompute_Events:" | cut -d' ' -f2-)
    local cache_budget=$(echo "$output" | grep "Param_3:" | cut -d' ' -f2-)
    local working_set=$(echo "$output" | grep "Working_Set_Proxy:" | cut -d' ' -f2-)
    local locality=$(echo "$output" | grep "Locality_Proxy:" | cut -d' ' -f2-)
    
    local backend_type=$(echo "$output" | grep "Backend:" | cut -d' ' -f2-)
    local transfer=$(echo "$output" | grep "Transfer_Overhead_us:" | cut -d' ' -f2-)
    local kernels=$(echo "$output" | grep "Kernel_Launch_Count:" | cut -d' ' -f2-)
    local device_budget=$(echo "$output" | grep "Device_Memory_Budget_kb:" | cut -d' ' -f2-)

    [ -z "$recompute" ] && recompute=0
    [ -z "$working_set" ] && working_set=0
    [ -z "$locality" ] && locality=0
    [ -z "$cache_budget" ] && cache_budget=0
    [ -z "$backend_type" ] && backend_type="cpu"
    [ -z "$transfer" ] && transfer="NA"
    [ -z "$kernels" ] && kernels="NA"
    [ -z "$device_budget" ] && device_budget="NA"
    
    echo "$alg,$var,$PLATFORM,$runtime,$memory,$recompute,$cache_budget,$working_set,$locality,$p1,$p2,$p3,$backend_type,$device_budget,$transfer,$kernels,Success" >> $CSV_FILE
}

# Scalability Experiments: Multi-Backend sweeps
for backend in "SRF_FORCE_CPU=1" "SRF_FORCE_GPU=1"; do
    # 1. Needleman-Wunsch Scalability (up to 1000)
    for seq in 400 1000; do
        log_run "Needleman-Wunsch" "SRF-CacheAware" "nw_blocked" 20 "$seq" 64 "$backend" 2048
    done

    # 2. HMM Scalability (up to 5000)
    for seq in 1000 5000; do
        log_run "Forward" "SRF-LocalityAware" "forward_checkpoint" 20 "$seq" 1 "$backend" 2048
    done

    # 3. Graph-DP Scalability (up to 10000 nodes)
    for nodes in 2000 10000; do
        log_run "Graph-DP" "SRF-LocalityAware" "graph_recompute" 4 "$nodes" 1 "$backend" 2048
    done
done

echo "[SRF] Generating Phase 4 plots..."
python3 benchmarks/generate_plots.py

echo "[SRF] Benchmarks finished."
