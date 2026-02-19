import pandas as pd
import numpy as np

def compute_relative_cost(row):
    """
    Derives relative cost indicators based on arithmetic vs memory pressure.
    Cost Ratio = (Recompute Events) / (Base Compute Events)
    """
    try:
        recompute = float(row['recompute_events'])
        # compute_events represents total arithmetic operations (base + recompute)
        total_compute = float(row['compute_events'])
        
        if total_compute <= 0:
            return "INSUFFICIENT_DATA"
            
        base_compute = total_compute - recompute
        if base_compute <= 0:
            return "INSUFFICIENT_DATA"
            
        # The recompute overhead ratio
        ratio = recompute / base_compute
        return ratio
    except (ValueError, KeyError, ZeroDivisionError):
        return "INSUFFICIENT_DATA"
