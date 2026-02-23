#!/usr/bin/env bash
set -e

echo "[SRF] Starting Determinism Stress Validation..."

TMP_DIR="results/tmp_determinism"
mkdir -p $TMP_DIR

# Baseline run
export SRF_LOG_FILE="$TMP_DIR/run1.csv"
bash benchmarks/run_benchmarks.sh > /dev/null

# Secondary run
export SRF_LOG_FILE="$TMP_DIR/run2.csv"
bash benchmarks/run_benchmarks.sh > /dev/null

# Compare outputs, ignoring runtime_us and status
# We'll use cut to grab stable columns
# algorithm,variant,platform,peak_memory_kb,recompute_events,compute_events,memory_access_proxy,dispatch_overhead_proxy,cache_budget,working_set_proxy,locality_proxy,param_1,param_2,param_3,backend_type,granularity_type,granularity_unit_size,unit_recompute_events,unit_reuse_proxy,dataset_scale,input_size
cut -d, -f1,2,5,6,7,8,9,10,11,12,13,14,15,16,20,21,22,23,24,25 $TMP_DIR/run1.csv > $TMP_DIR/run1_stable.csv
cut -d, -f1,2,5,6,7,8,9,10,11,12,13,14,15,16,20,21,22,23,24,25 $TMP_DIR/run2.csv > $TMP_DIR/run2_stable.csv

if diff $TMP_DIR/run1_stable.csv $TMP_DIR/run2_stable.csv > /dev/null; then
    echo "[PASS] Bit-exact determinism verified."
else
    echo "[FAIL] Determinism violation detected!"
    diff $TMP_DIR/run1_stable.csv $TMP_DIR/run2_stable.csv
    exit 1
fi
