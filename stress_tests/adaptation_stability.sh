#!/usr/bin/env bash
set -e

echo "[SRF] Starting Adaptation Stability Validation..."

CSV_FILE="results/csv/adaptation_validation.csv"
PLATFORM=$(uname)
mkdir -p results/csv
echo "algorithm,platform,adaptation_count,stability_status" > $CSV_FILE

run_stability() {
    local bin=$1
    local alg=$2
    local arg1=$3
    local arg2=$4
    local arg3=$5
    local arg4=$6
    local arg5=$7
    
    echo "Probing $alg stability..."
    local output=$(./build/$bin "$arg1" "$arg2" "$arg3" "$arg4" "$arg5")
    local adapt_count=$(echo "$output" | grep -c "ADAPTATION_EVENT: true")
    
    # Heuristic: Adaptation should be rare (< 5 events for these scales)
    local status="STABLE"
    if [ $adapt_count -gt 5 ]; then
        status="OSCILLATING"
    fi
    
    echo "$alg,$PLATFORM,$adapt_count,$status" >> $CSV_FILE
}

# 1. NW Skewed Stability
run_stability nw_blocked "Needleman-Wunsch" "datasets_extreme/pathological/seq_skewed_long.txt" "datasets_extreme/pathological/seq_skewed_short.txt" 20 1 "SKEWED"

# 2. Graph Dense Stability
run_stability graph_recompute "Graph-DP" "datasets_extreme/pathological/graph_dense.txt" 4 1 "DENSE" ""

echo "[SRF] Adaptation stability checks finished."
