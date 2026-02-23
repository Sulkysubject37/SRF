#!/usr/bin/env bash
set -e

echo "[SRF] Starting Phase 8-A Observation Benchmarks..."

# Ensure we are in the root directory
cd "$(dirname "$0")/.."

# Build all binaries
bash benchmarks/build.sh

# CSV Schema Extension for Phase 8-A
# algorithm,variant,platform,runtime_us,peak_memory_kb,recompute_events,compute_events,memory_access_proxy,dispatch_overhead_proxy,cache_budget,working_set_proxy,locality_proxy,param_1,param_2,param_3,backend_type,device_memory_budget,transfer_overhead_us,kernel_launch_count,granularity_type,granularity_unit_size,unit_recompute_events,unit_reuse_proxy,dataset_scale,input_size,regime_state,drift_state,R_mem,R_rec,status
CSV_FILE="results/csv/benchmark_log.csv"
PLATFORM=$(uname)

mkdir -p results/csv
echo "algorithm,variant,platform,runtime_us,peak_memory_kb,recompute_events,compute_events,memory_access_proxy,dispatch_overhead_proxy,cache_budget,working_set_proxy,locality_proxy,param_1,param_2,param_3,backend_type,device_memory_budget,transfer_overhead_us,kernel_launch_count,granularity_type,granularity_unit_size,unit_recompute_events,unit_reuse_proxy,dataset_scale,input_size,regime_state,drift_state,R_mem,R_rec,status" > $CSV_FILE

# Helper to log a run
log_run() {
    local alg=$1
    local var=$2
    local bin=$3
    local arg1=$4 # PathA / Path
    local arg2=$5 # PathB / K / Depth
    local arg3=$6 # B / G / G
    local arg4=$7 # G / Scale / Scale
    local arg5=$8 # Scale / "" / ""
    local backend_env=$9
    
    echo "Running $alg ($var) Scale=$arg5 Backend=$backend_env..."
    local output=$(env $backend_env ./build/$bin "$arg1" "$arg2" "$arg3" "$arg4" "$arg5")
    
    local runtime=$(echo "$output" | grep -w "Time_us:" | cut -d' ' -f2- | tr -d '\r')
    local memory=$(echo "$output" | grep -w "Memory_kb:" | cut -d' ' -f2- | tr -d '\r')
    local recompute=$(echo "$output" | grep -w "Recompute_Events:" | cut -d' ' -f2- | tr -d '\r')
    local compute=$(echo "$output" | grep -w "Compute_Events:" | cut -d' ' -f2- | tr -d '\r')
    local mem_proxy=$(echo "$output" | grep -w "Memory_Access_Proxy:" | cut -d' ' -f2- | tr -d '\r')
    local dispatch=$(echo "$output" | grep -w "Dispatch_Overhead_Proxy:" | cut -d' ' -f2- | tr -d '\r')
    local unit_recompute=$(echo "$output" | grep -w "Unit_Recompute_Events:" | cut -d' ' -f2- | tr -d '\r')
    local unit_reuse=$(echo "$output" | grep -w "Unit_Reuse_Proxy:" | cut -d' ' -f2- | tr -d '\r')
    local unit_size=$(echo "$output" | grep -w "Granularity_Unit_Size:" | cut -d' ' -f2- | tr -d '\r')
    local working_set=$(echo "$output" | grep -w "Working_Set_Proxy:" | cut -d' ' -f2- | tr -d '\r')
    local backend_type=$(echo "$output" | grep -w "Backend:" | cut -d' ' -f2- | tr -d '\r')
    local input_size=$(echo "$output" | grep -w "Input_Size:" | cut -d' ' -f2- | tr -d '\r')
    local scale=$(echo "$output" | grep -w "Dataset_Scale:" | cut -d' ' -f2- | tr -d '\r')
    
    # Phase 8A specific metrics
    local drift_state=$(echo "$output" | grep -w "Drift_State:" | cut -d' ' -f2- | tr -d '\r')
    local r_mem=$(echo "$output" | grep -w "R_mem:" | cut -d' ' -f2- | tr -d '\r')
    local r_rec=$(echo "$output" | grep -w "R_rec:" | cut -d' ' -f2- | tr -d '\r')

    [ -z "$recompute" ] && recompute=0
    [ -z "$compute" ] && compute=0
    [ -z "$mem_proxy" ] && mem_proxy=0
    [ -z "$dispatch" ] && dispatch=0
    [ -z "$unit_recompute" ] && unit_recompute=0
    [ -z "$unit_reuse" ] && unit_reuse=0
    [ -z "$unit_size" ] && unit_size="NA"
    [ -z "$working_set" ] && working_set=0
    [ -z "$backend_type" ] && backend_type="cpu"
    [ -z "$input_size" ] && input_size=0
    [ -z "$scale" ] && scale="NA"
    [ -z "$drift_state" ] && drift_state="INSUFFICIENT_DATA"
    [ -z "$r_mem" ] && r_mem=0
    [ -z "$r_rec" ] && r_rec=0
    
    local g_type="unknown"
    [[ "$alg" == "Needleman-Wunsch" ]] && g_type="tile"
    [[ "$alg" == "Forward" || "$alg" == "Viterbi" ]] && g_type="segment"
    [[ "$alg" == "Graph-DP" ]] && g_type="group"

    echo "$alg,$var,$PLATFORM,$runtime,$memory,$recompute,$compute,$mem_proxy,$dispatch,0,$working_set,0,0,0,0,$backend_type,NA,NA,NA,$g_type,$unit_size,$unit_recompute,$unit_reuse,$scale,$input_size,UNKNOWN,$drift_state,$r_mem,$r_rec,Success" >> $CSV_FILE
}

# Standard Sweeps (using XS/S for rapid 8A verification)
for backend in "SRF_FORCE_CPU=1"; do
    # NW XS
    log_run "Needleman-Wunsch" "SRF-GranularityAware" "nw_blocked" "datasets/sequences/processed/human_xs.txt" "datasets/sequences/processed/neand_xs.txt" 20 1 "XS" "$backend"
    # Forward S
    log_run "Forward" "SRF-GranularityAware" "forward_checkpoint" "datasets/sequences/processed/human_s.txt" 20 1 "S" "" "$backend"
    # Graph S
    log_run "Graph-DP" "SRF-GranularityAware" "graph_recompute" "datasets/graphs/processed/go_subset_s.txt" 4 1 "S" "" "$backend"
done

echo "[SRF] Generating Phase 8-A reports..."
python3 analysis/predictive_analysis.py
python3 analysis/regime_mapping.py

echo "[SRF] Benchmarks finished."
