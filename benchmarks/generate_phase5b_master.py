import pandas as pd
import matplotlib.pyplot as plt
import numpy as np

def generate_phase5b_master():
    df = pd.read_csv('docs/phase_5b_results_consolidated.csv')
    plt.style.use('seaborn-v0_8-whitegrid')
    plt.rcParams.update({'font.size': 12, 'font.family': 'sans-serif'})
    
    fig, (ax1, ax2) = plt.subplots(1, 2, figsize=(20, 9))
    fig.patch.set_facecolor('#ffffff')

    # 1. Model Explanation: Relative Cost Ratio (Bar)
    # This is an algorithmic property, stable across OS
    algs = ['Needleman-Wunsch', 'Forward', 'Graph-DP']
    ratios = [df[df['algorithm'] == a]['relative_cost_ratio'].iloc[0] for a in algs]
    
    ax1.bar(algs, ratios, color=['#3498db', '#2ecc71', '#e74c3c'], alpha=0.8, edgecolor='black')
    ax1.set_ylabel('Model Predicted Cost Ratio (Recompute/Base)', fontsize=14, fontweight='bold')
    ax1.set_title('A: Algorithmic Recomputation Cost Profile', fontsize=16, fontweight='bold', pad=20)
    
    # Annotate the ratios
    for i, v in enumerate(ratios):
        ax1.text(i, v + 0.1, f"{v:.2f}x", ha='center', fontweight='bold')

    # 2. Locality Model: Locality Index vs Granularity
    # Filter for Darwin/CPU to show the trend
    darwin_df = df[df['platform'] == 'Darwin'].sort_values('granularity_unit_size')
    
    colors = {'Needleman-Wunsch': '#1f77b4', 'Forward': '#2ca02c', 'Graph-DP': '#d62728'}
    for alg in algs:
        a_df = darwin_df[darwin_df['algorithm'] == alg]
        if not a_df.empty:
            ax2.plot(a_df['granularity_unit_size'], a_df['locality_index'], 'o-', 
                     label=alg, color=colors[alg], linewidth=2.5, markersize=8)

    ax2.set_xlabel('Granularity Unit Size (G)', fontsize=14, fontweight='bold')
    ax2.set_ylabel('Model Locality Index (Reuse / G)', fontsize=14, fontweight='bold')
    ax2.set_title('B: Predicted Temporal Locality Trend', fontsize=16, fontweight='bold', pad=20)
    ax2.legend(frameon=True, shadow=True)
    ax2.grid(True, linestyle='--', alpha=0.6)

    plt.tight_layout(rect=[0, 0.03, 1, 0.95])
    plt.savefig('docs/phase_5_b_global_master.png', dpi=300, bbox_inches='tight')
    print("Analytical master plot saved: docs/phase_5_b_global_master.png")

if __name__ == "__main__":
    generate_phase5b_master()
