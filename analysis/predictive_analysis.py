import pandas as pd
import os
import sys
from models.cost_model import compute_relative_cost
from models.cache_model import predict_cache_regime
from models.accelerator_model import predict_accelerator_feasibility
from models.working_set_model import compute_locality_index

def run_predictive_analysis(input_csv, output_csv):
    if not os.path.exists(input_csv):
        print(f"Error: {input_csv} not found.")
        sys.exit(1)

    df = pd.read_csv(input_csv)
    
    # Ensure mandatory fields exist for analysis
    # If they are missing from C++, we fill with NA to avoid crashes
    required_fields = [
        'compute_events', 'recompute_events', 'memory_access_proxy', 
        'granularity_unit_size', 'backend_type', 'dispatch_overhead_proxy', 
        'working_set_proxy', 'cache_budget', 'unit_reuse_proxy'
    ]
    
    for field in required_fields:
        if field not in df.columns:
            df[field] = "NA"

    results = []
    for _, row in df.iterrows():
        prediction = {
            'algorithm': row['algorithm'],
            'variant': row['variant'],
            'platform': row['platform'],
            'backend_type': row['backend_type'],
            'granularity_unit_size': row['granularity_unit_size'],
            'relative_cost_ratio': compute_relative_cost(row),
            'cache_regime': predict_cache_regime(row),
            'accelerator_feasibility': predict_accelerator_feasibility(row),
            'locality_index': compute_locality_index(row)
        }
        results.append(prediction)

    out_df = pd.DataFrame(results)
    os.makedirs(os.path.dirname(output_csv), exist_ok=True)
    out_df.to_csv(output_csv, index=False)
    print(f"Analytical predictions saved to {output_csv}")

if __name__ == "__main__":
    input_file = "results/csv/benchmark_log.csv"
    output_file = "results/csv/predictions.csv"
    run_predictive_analysis(input_file, output_file)
