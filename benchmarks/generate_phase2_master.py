import pandas as pd
import matplotlib.pyplot as plt
import numpy as np

def generate_phase2_master():
    df = pd.read_csv('docs/phase_2_results_complete.csv')
    
    algorithms = ['Needleman-Wunsch', 'Forward', 'Viterbi', 'Graph-DP']
    platforms = ['Darwin', 'Linux', 'Windows']
    
    plt.style.use('bmh')
    fig, (ax1, ax2) = plt.subplots(1, 2, figsize=(22, 10))
    fig.suptitle('Phase 2: Global Cross-Platform SRF vs Baseline Performance Profile', fontsize=20, fontweight='bold')

    bar_width = 0.12
    # Group indices: for each alg, we'll have 3 platforms, each with 2 bars.
    # We'll set x-ticks for each algorithm.
    index = np.arange(len(algorithms))
    
    # Offsets for Darwin (B/S), Linux (B/S), Windows (B/S)
    offsets = [-0.3, -0.18, -0.06, 0.06, 0.18, 0.3]
    labels = ['Darwin (B)', 'Darwin (S)', 'Linux (B)', 'Linux (S)', 'Windows (B)', 'Windows (S)']
    colors = ['#1f77b4', '#aec7e8', '#2ca02c', '#98df8a', '#d62728', '#ff9896']

    for i, alg in enumerate(algorithms):
        alg_df = df[df['algorithm'] == alg]
        
        for j, plat in enumerate(platforms):
            p_df = alg_df[alg_df['platform'] == plat]
            
            # Baseline
            b_val_rt = p_df[p_df['variant'] == 'Baseline']['runtime_us'].values[0]
            b_val_mem = p_df[p_df['variant'] == 'Baseline']['peak_memory_kb'].values[0]
            
            # SRF
            s_val_rt = p_df[p_df['variant'].str.contains('SRF')]['runtime_us'].values[0]
            s_val_mem = p_df[p_df['variant'].str.contains('SRF')]['peak_memory_kb'].values[0]
            
            # Runtime (ax1)
            ax1.bar(i + offsets[j*2], b_val_rt, bar_width, color=colors[j*2], edgecolor='black', alpha=0.9)
            ax1.bar(i + offsets[j*2+1], s_val_rt, bar_width, color=colors[j*2+1], edgecolor='black', alpha=0.9)
            
            # Memory (ax2)
            ax2.bar(i + offsets[j*2], b_val_mem, bar_width, color=colors[j*2], edgecolor='black', alpha=0.9)
            ax2.bar(i + offsets[j*2+1], s_val_mem, bar_width, color=colors[j*2+1], edgecolor='black', alpha=0.9)

    # 1. Runtime Subplot
    ax1.set_title('Runtime (µs) - Log Scale', fontsize=16, fontweight='bold')
    ax1.set_yscale('log')
    ax1.set_xticks(index)
    ax1.set_xticklabels(algorithms, fontsize=12, fontweight='bold')
    ax1.set_ylabel('Execution Time (µs)')
    
    # 2. Memory Subplot
    ax2.set_title('Peak Memory Usage (KB)', fontsize=16, fontweight='bold')
    ax2.set_xticks(index)
    ax2.set_xticklabels(algorithms, fontsize=12, fontweight='bold')
    ax2.set_ylabel('Peak Memory (KB)')

    # Add Legend to figure
    from matplotlib.lines import Line2D
    legend_elements = [Line2D([0], [0], color=colors[k], lw=4, label=labels[k]) for k in range(6)]
    fig.legend(handles=legend_elements, loc='upper right', bbox_to_anchor=(0.95, 0.95), fontsize=12, title="Platform (B=Baseline, S=SRF)")

    plt.tight_layout(rect=[0, 0.03, 1, 0.92])
    plt.savefig('docs/phase_2_global_master.png', dpi=300)
    print("Complete Phase 2 master plot saved to docs/phase_2_global_master.png")

if __name__ == "__main__":
    generate_phase2_master()