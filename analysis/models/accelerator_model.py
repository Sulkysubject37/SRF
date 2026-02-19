import pandas as pd

def predict_accelerator_feasibility(row):
    """
    Evaluates accelerator feasibility based on task granularity.
    If unit size is large, dispatch overhead is amortized.
    """
    try:
        granularity = float(row['granularity_unit_size'])
        overhead = float(row['dispatch_overhead_proxy'])
        
        if row['backend_type'] == 'cpu' and overhead == 0:
            # For CPU runs, we look at the potential for GPU
            if granularity >= 32:
                return "FEASIBLE"
            elif granularity > 1:
                return "MARGINAL"
            else:
                return "UNFEASIBLE"
        
        return "OBSERVED"
    except (ValueError, KeyError):
        return "INSUFFICIENT_DATA"
