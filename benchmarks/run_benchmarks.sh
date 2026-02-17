#!/usr/bin/env bash
set -e

echo "[SRF] Starting Benchmarks..."

# Ensure we are in the root directory
cd "$(dirname "$0")/.."

# Build first
bash benchmarks/build.sh

# Measurements recorded to benchmark_log.csv
# Format: Algorithm,Variant,Platform,Time_us,Status
CSV_FILE="results/csv/benchmark_log.csv"
PLATFORM=$(uname)

mkdir -p results/csv
echo "Algorithm,Variant,Platform,Time_us,Status" > $CSV_FILE

# Execute binaries and capture output
for binary in build/*; do
    # Skip directories
    [ -d "$binary" ] && continue
    
    echo "Running $(basename $binary)..."
    output=$(./$binary)
    
    # Parse output using grep/sed
    algorithm=$(echo "$output" | grep "Algorithm:" | cut -d' ' -f2-)
    time_us=$(echo "$output" | grep "Time_us:" | cut -d' ' -f2-)
    
    echo "$algorithm,Baseline,$PLATFORM,$time_us,Success" >> $CSV_FILE
done

echo "[SRF] Generating performance plots..."
# Assuming python3 and requirements (pandas, matplotlib) are available in the environment
# The CI workflow will be updated to ensure these are present.
python3 benchmarks/plot_results.py

echo "[SRF] Benchmarks finished."
