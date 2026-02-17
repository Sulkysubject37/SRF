#!/usr/bin/env bash
set -e

echo "[SRF] Starting Correctness Tests..."

# Ensure we are in the root directory
cd "$(dirname "$0")/.."

# Build all binaries
bash benchmarks/build.sh

# Helper to run binaries with optional .exe
run_bin() {
    if [ -f "./build/$1.exe" ]; then
        ./build/$1.exe "$2" "$3"
    elif [ -f "./build/$1" ]; then
        ./build/$1 "$2" "$3"
    else
        echo "[ERROR] Binary $1 not found in build/"
        exit 1
    fi
}

# 1. Needleman-Wunsch Equivalence
BASE_NW=$(run_bin needleman_wunsch | grep "Final_Score_Check:" | cut -d' ' -f2-)
SRF_NW=$(run_bin nw_blocked 200 20 | grep "Score:" | cut -d' ' -f2-)
if [ "$BASE_NW" != "$SRF_NW" ]; then
    echo "[FAIL] Needleman-Wunsch: Baseline $BASE_NW != SRF $SRF_NW"
    exit 1
fi
echo "[PASS] Needleman-Wunsch Equivalence"

# 2. Viterbi Equivalence
BASE_V=$(run_bin viterbi | grep "Viterbi_Check:" | cut -d' ' -f2-)
SRF_V=$(run_bin viterbi_checkpoint 500 10 | grep "Result:" | cut -d' ' -f2-)
if [ "$BASE_V" != "$SRF_V" ]; then
    echo "[FAIL] Viterbi: Baseline $BASE_V != SRF $SRF_V"
    exit 1
fi
echo "[PASS] Viterbi Equivalence"

# 3. Forward Equivalence
BASE_F=$(run_bin forward | grep "Forward_Check:" | cut -d' ' -f2-)
SRF_F=$(run_bin forward_checkpoint 500 10 | grep "Result:" | cut -d' ' -f2-)
if [ "$BASE_F" != "$SRF_F" ]; then
    echo "[FAIL] Forward: Baseline $BASE_F != SRF $SRF_F"
    exit 1
fi
echo "[PASS] Forward Equivalence"

# 4. Graph-DP Equivalence
BASE_G=$(run_bin graph_dp | grep "Graph_Check:" | cut -d' ' -f2-)
SRF_G=$(run_bin graph_recompute 1000 2 | grep "Result:" | cut -d' ' -f2-)
if [ "$BASE_G" != "$SRF_G" ]; then
    echo "[FAIL] Graph-DP: Baseline $BASE_G != SRF $SRF_G"
    exit 1
fi
echo "[PASS] Graph-DP Equivalence"

echo "[SRF] All tests passed."