import pandas as pd
import matplotlib.pyplot as plt
import numpy as np
import os

def generate_phase6_master():
    df = pd.read_csv('docs/phase_6_results_consolidated.csv')
    plt.style.use('seaborn-v0_8-whitegrid')
    plt.rcParams.update({'font.size': 12, 'font.family': 'sans-serif'})
    
    fig, (ax1, ax2) = plt.subplots(1, 2, figsize=(20, 9))
    fig.patch.set_facecolor('#ffffff')

    # 1. Subplot A: Runtime Scaling Complexity (XS to XL)
    # Filter for Darwin as the primary scaling reference
    darwin_df = df[df['platform'] == 'Darwin']
    
    algs = ['Needleman-Wunsch', 'Forward']
    colors = {'Needleman-Wunsch': '#1f77b4', 'Forward': '#2ca02c'}
    markers = {'Needleman-Wunsch': 'o', 'Forward': 's'}
    
    for alg in algs:
        a_df = darwin_df[darwin_df['algorithm'] == alg].sort_values('input_size')
        if not a_df.empty:
            ax1.plot(a_df['input_size'], a_df['runtime_us'], marker=markers[alg], 
                     color=colors[alg], linewidth=3, markersize=10, label=f'{alg} (Observed)')

    # Add Complexity Reference Lines
    x_range = np.linspace(200, 4000, 100)
    # Normalize O(N^2) to match NW XS
    nw_xs = darwin_df[darwin_df['algorithm'] == 'Needleman-Wunsch']['runtime_us'].iloc[0]
    ax1.plot(x_range, nw_xs * (x_range/200)**2, '--', color='#1f77b4', alpha=0.4, label='O(N²) Reference')
    
    # Normalize O(N) to match Forward XS
    f_xs = darwin_df[darwin_df['algorithm'] == 'Forward']['runtime_us'].iloc[0]
    ax1.plot(x_range, f_xs * (x_range/200), '--', color='#2ca02c', alpha=0.4, label='O(N) Reference')

    ax1.set_xscale('log')
    ax1.set_yscale('log')
    ax1.set_xlabel('Input Size (Sequence Length)', fontsize=14, fontweight='bold')
    ax1.set_ylabel('Execution Time (µs, Log Scale)', fontsize=14, fontweight='bold')
    ax1.set_title('A: Biological Workload Scaling Complexity', fontsize=16, fontweight='bold', pad=20)
    ax1.legend(frameon=True, shadow=True)
    ax1.grid(True, which="both", ls="-", alpha=0.2)

    # 2. Subplot B: Model Validation (Predicted Cost Ratio Stability)
    # Group by algorithm and scale to show invariance
    for alg in algs:
        a_df = df[df['algorithm'] == alg].sort_values('input_size')
        ax2.plot(a_df['input_size'], a_df['relative_cost_ratio'], marker=markers[alg], 
                 color=colors[alg], linewidth=2, markersize=8, label=alg)

    ax2.set_xscale('log')
    ax2.set_xlabel('Input Size', fontsize=14, fontweight='bold')
    ax2.set_ylabel('Relative Cost Ratio (Algebraic)', fontsize=14, fontweight='bold')
    ax2.set_ylim(0, 1.5)
    ax2.set_title('B: Model Invariance across Biological Scales', fontsize=16, fontweight='bold', pad=20)
    ax2.legend(frameon=True, shadow=True)
    ax2.grid(True, which="both", ls="-", alpha=0.2)

    plt.tight_layout(rect=[0, 0.03, 1, 0.95])
    plt.savefig('docs/phase_6_global_master.png', dpi=300, bbox_inches='tight')
    print("Phase 6 Master plot saved: docs/phase_6_global_master.png")

if __name__ == "__main__":
    generate_phase6_master()
