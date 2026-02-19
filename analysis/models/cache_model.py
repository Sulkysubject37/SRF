import pandas as pd

def predict_cache_regime(row):
    """
    Predicts if the working set fits within the provided cache budget.
    """
    try:
        ws = float(row['working_set_proxy'])
        budget = float(row['cache_budget'])
        
        if budget == 0:
            return "UNKNOWN"
            
        # ws is usually in bytes, budget in KB in some contexts, 
        # but here we assume both are in consistent units or provided as parameters.
        # Based on srf_utils.hpp, budget is in bytes.
        
        if ws <= budget:
            return "FIT"
        else:
            return "SPILL"
    except (ValueError, KeyError):
        return "INSUFFICIENT_DATA"
