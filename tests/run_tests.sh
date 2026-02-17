#!/usr/bin/env bash
set -e

echo "[SRF] Starting Correctness Tests..."

# Ensure we are in the root directory
cd "$(dirname "$0")/.."

# Ensure binaries are built
bash benchmarks/build.sh

# Helper to run binaries with optional .exe
run_bin() {
    if [ -f "./build/$1.exe" ]; then
        ./build/$1.exe
    elif [ -f "./build/$1" ]; then
        ./build/$1
    else
        echo "[ERROR] Binary $1 not found in build/"
        exit 1
    fi
}

# Verifying deterministic outputs
# Needleman-Wunsch GATTACA/GCATGCU score should be 0
NW_OUTPUT=$(run_bin needleman_wunsch | grep "Final_Score:" | cut -d' ' -f2-)
if [ "$NW_OUTPUT" != "0" ]; then
    echo "[FAIL] Needleman-Wunsch: Expected score 0, got $NW_OUTPUT"
    exit 1
fi
echo "[PASS] Needleman-Wunsch"

# Viterbi Algorithm
V_OUTPUT=$(run_bin viterbi | grep "Max_Probability:" | cut -d' ' -f2-)
if [ -z "$V_OUTPUT" ]; then
    echo "[FAIL] Viterbi produced no output"
    exit 1
fi
echo "[PASS] Viterbi"

# Forward Algorithm
F_OUTPUT=$(run_bin forward | grep "Total_Probability:" | cut -d' ' -f2-)
if [ -z "$F_OUTPUT" ]; then
    echo "[FAIL] Forward produced no output"
    exit 1
fi
echo "[PASS] Forward"

# Graph DP
GD_OUTPUT=$(run_bin graph_dp | grep "Final_Distance:" | cut -d' ' -f2-)
if [ "$GD_OUTPUT" != "4" ]; then
    echo "[FAIL] Graph DP: Expected distance 4, got $GD_OUTPUT"
    exit 1
fi
echo "[PASS] Graph DP"

echo "[SRF] All tests passed."