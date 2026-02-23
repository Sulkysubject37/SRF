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

# Compare outputs, ignoring runtime_us, platform, peak_memory_kb and status
# Stable Columns (indices in original CSV):
# 1: algorithm, 2: variant, 6: recompute_events, 7: compute_events, 
# 8: memory_access_proxy, 9: dispatch_overhead_proxy, 10: cache_budget,
# 11: working_set_proxy, 12: locality_proxy, 13: param_1, 14: param_2, 
# 15: param_3, 16: backend_type, 20: granularity_type, 21: granularity_unit_size, 
# 22: unit_recompute_events, 23: unit_reuse_proxy, 24: dataset_scale, 25: input_size
cut -d, -f1,2,6,7,8,9,10,11,12,13,14,15,16,20,21,22,23,24,25 $TMP_DIR/run1.csv > $TMP_DIR/run1_stable.csv
cut -d, -f1,2,6,7,8,9,10,11,12,13,14,15,16,20,21,22,23,24,25 $TMP_DIR/run2.csv > $TMP_DIR/run2_stable.csv

if diff $TMP_DIR/run1_stable.csv $TMP_DIR/run2_stable.csv > /dev/null; then
    echo "[PASS] Bit-exact determinism verified."
else
    echo "[FAIL] Determinism violation detected!"
    diff $TMP_DIR/run1_stable.csv $TMP_DIR/run2_stable.csv
    exit 1
fi
