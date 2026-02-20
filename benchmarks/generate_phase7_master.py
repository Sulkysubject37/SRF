import pandas as pd
import matplotlib.pyplot as plt
import numpy as np

def generate_phase7_master():
    df = pd.read_csv('docs/phase_7_results_consolidated.csv')
    plt.style.use('seaborn-v0_8-whitegrid')
    plt.rcParams.update({'font.size': 12, 'font.family': 'sans-serif'})
    
    fig, (ax1, ax2) = plt.subplots(1, 2, figsize=(20, 9))
    fig.patch.set_facecolor('#ffffff')

    # 1. Subplot A: Extreme Scaling Complexity (Darwin vs Linux)
    # We'll plot both platforms to show stability
    algs = ['Needleman-Wunsch', 'Forward', 'Graph-DP']
    colors = {'Needleman-Wunsch': '#1f77b4', 'Forward': '#2ca02c', 'Graph-DP': '#d62728'}
    markers = {'Darwin': 'o', 'Linux': 's'}
    
    for alg in algs:
        for plat in ['Darwin', 'Linux']:
            a_df = df[(df['algorithm'] == alg) & (df['platform'] == plat)].sort_values('input_size')
            if not a_df.empty:
                ax1.plot(a_df['input_size'], a_df['runtime_us'], 
                         marker=markers[plat], color=colors[alg], linewidth=2.5, 
                         markersize=8, alpha=0.7, label=f'{alg} ({plat})')

    ax1.set_xscale('log')
    ax1.set_yscale('log')
    ax1.set_xlabel('Input Size (Length / Nodes)', fontsize=14, fontweight='bold')
    ax1.set_ylabel('Runtime (µs, Log Scale)', fontsize=14, fontweight='bold')
    ax1.set_title('A: Extreme Scale Runtime Comparison', fontsize=16, fontweight='bold', pad=20)
    ax1.legend(frameon=True, shadow=True, ncol=2)
    ax1.grid(True, which="both", ls="-", alpha=0.2)

    # 2. Subplot B: Arithmetic vs Memory Pressure (Regime Analysis)
    # Plotting Memory Accesses vs Recompute Events to visualize regimes
    for alg in algs:
        a_df = df[df['algorithm'] == alg]
        if not a_df.empty:
            ax2.scatter(a_df['recompute_events'], a_df['memory_access_proxy'], 
                        s=150, color=colors[alg], alpha=0.6, edgecolors='black', label=alg)

    ax2.set_xscale('log')
    ax2.set_yscale('log')
    
    # Draw Regime Boundaries (Conceptual)
    lims = [1e1, 1e10]
    ax2.plot(lims, lims, '--', color='gray', alpha=0.5, label='Balanced Line (1:1)')
    
    ax2.set_xlabel('Recomputation Events (Arithmetic Load)', fontsize=14, fontweight='bold')
    ax2.set_ylabel('Memory Access Proxy (Bandwidth Load)', fontsize=14, fontweight='bold')
    ax2.set_title('B: Execution Regime Mapping', fontsize=16, fontweight='bold', pad=20)
    ax2.legend(frameon=True, shadow=True)
    
    # Annotations for regimes
    ax2.text(1e8, 1e9, "MEMORY BOUND", rotation=45, ha='center', fontsize=10, style='italic', color='#1f77b4')
    ax2.text(1e5, 1e4, "COMPUTE BOUND", rotation=45, ha='center', fontsize=10, style='italic', color='#d62728')

    plt.tight_layout(rect=[0, 0.03, 1, 0.95])
    plt.savefig('docs/phase_7_global_master.png', dpi=300, bbox_inches='tight')
    print("Extreme master plot saved: docs/phase_7_global_master.png")

if __name__ == "__main__":
    generate_phase7_master()
