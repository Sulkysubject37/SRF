#!/usr/bin/env bash
set -e

echo "[SRF] Starting Correctness Tests (Real-world Biological Data)..."

# Ensure we are in the root directory
cd "$(dirname "$0")/.."

# Build all binaries
bash benchmarks/build.sh

# Helper to run binaries with optional .exe
run_bin() {
    if [ -f "./build/$1.exe" ]; then
        ./build/$1.exe "$2" "$3" "$4" "$5" "$6"
    elif [ -f "./build/$1" ]; then
        ./build/$1 "$2" "$3" "$4" "$5" "$6"
    else
        echo "[ERROR] Binary $1 not found in build/"
        exit 1
    fi
}

# Real Data Paths (XS Scale)
SEQ_H="datasets/sequences/processed/human_xs.txt"
SEQ_N="datasets/sequences/processed/neand_xs.txt"
GRAPH_O="datasets/graphs/processed/go_subset_xs.txt"

# 1. Needleman-Wunsch Equivalence
BASE_NW=$(run_bin needleman_wunsch "$SEQ_H" "$SEQ_N" | grep "Result_Check:" | cut -d' ' -f2- | tr -d '\r')
SRF_NW=$(run_bin nw_blocked "$SEQ_H" "$SEQ_N" 20 1 XS | grep "Result_Check:" | cut -d' ' -f2- | tr -d '\r')
if [ "$BASE_NW" != "$SRF_NW" ]; then
    echo "[FAIL] Needleman-Wunsch: Baseline $BASE_NW != SRF $SRF_NW"
    exit 1
fi
echo "[PASS] Needleman-Wunsch Equivalence"

# 2. Viterbi Equivalence
BASE_V=$(run_bin viterbi "$SEQ_H" | grep "Result_Check:" | cut -d' ' -f2- | tr -d '\r')
SRF_V=$(run_bin viterbi_checkpoint "$SEQ_H" 10 1 XS | grep "Result_Check:" | cut -d' ' -f2- | tr -d '\r')
if [ "$BASE_V" != "$SRF_V" ]; then
    echo "[FAIL] Viterbi: Baseline $BASE_V != SRF $SRF_V"
    exit 1
fi
echo "[PASS] Viterbi Equivalence"

# 3. Forward Equivalence
BASE_F=$(run_bin forward "$SEQ_H" | grep "Result_Check:" | cut -d' ' -f2- | tr -d '\r')
SRF_F=$(run_bin forward_checkpoint "$SEQ_H" 10 1 XS | grep "Result_Check:" | cut -d' ' -f2- | tr -d '\r')
if [ "$BASE_F" != "$SRF_F" ]; then
    echo "[FAIL] Forward: Baseline $BASE_F != SRF $SRF_F"
    exit 1
fi
echo "[PASS] Forward Equivalence"

# 4. Graph-DP Equivalence
BASE_G=$(run_bin graph_dp "$GRAPH_O" | grep "Result_Check:" | cut -d' ' -f2- | tr -d '\r')
SRF_G=$(run_bin graph_recompute "$GRAPH_O" 2 1 XS | grep "Result_Check:" | cut -d' ' -f2- | tr -d '\r')
if [ "$BASE_G" != "$SRF_G" ]; then
    echo "[FAIL] Graph-DP: Baseline $BASE_G != SRF $SRF_G"
    exit 1
fi
echo "[PASS] Graph-DP Equivalence"

echo "[SRF] All tests passed."
