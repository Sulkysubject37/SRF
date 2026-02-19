import pandas as pd

def compute_locality_index(row):
    """
    Computes a relative locality index based on unit reuse.
    Locality Index = Unit Reuse Proxy / Granularity Unit Size
    """
    try:
        reuse = float(row['unit_reuse_proxy'])
        g_size = float(row['granularity_unit_size'])
        
        if g_size <= 0:
            return "INSUFFICIENT_DATA"
            
        index = reuse / g_size
        return index
    except (ValueError, KeyError):
        return "INSUFFICIENT_DATA"
