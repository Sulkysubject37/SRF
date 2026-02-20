import pandas as pd
import os
import sys

def classify_regime(row):
    try:
        runtime = float(row['runtime_us'])
        recompute = float(row['recompute_events'])
        compute = float(row['compute_events'])
        mem_proxy = float(row['memory_access_proxy'])
        
        # 1. Recomputation Dominated: Recompute events > 50% of total compute
        if recompute > (compute * 0.5):
            return "RECOMPUTATION_DOMINATED"
        
        # 2. Compute Bound: Compute events / Memory Proxy is high
        if compute > (mem_proxy * 2):
            return "COMPUTE_BOUND"
            
        # 3. Memory Bound: Memory Accesses > Compute events
        if mem_proxy > compute:
            return "MEMORY_BOUND"
            
        return "BALANCED"
    except:
        return "UNKNOWN"

def main():
    input_csv = "results/csv/extreme_log.csv"
    output_csv = "results/csv/regime_mapping.csv"
    
    if not os.path.exists(input_csv):
        print("Log not found.")
        return

    df = pd.read_csv(input_csv)
    df['observed_regime'] = df.apply(classify_regime, axis=1)
    
    os.makedirs(os.path.dirname(output_csv), exist_ok=True)
    df.to_csv(output_csv, index=False)
    print(f"Regime mapping saved to {output_csv}")

if __name__ == "__main__":
    main()
