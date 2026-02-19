import pandas as pd
import matplotlib.pyplot as plt
import numpy as np

def generate_phase5_master():
    df = pd.read_csv('docs/phase_5_results_consolidated.csv')
    
    # Set high-fidelity style
    plt.style.use('seaborn-v0_8-whitegrid')
    plt.rcParams.update({'font.size': 12, 'font.family': 'sans-serif'})
    
    fig, (ax1, ax2) = plt.subplots(1, 2, figsize=(20, 9))
    fig.patch.set_facecolor('#ffffff')
    
    # 1. Subplot A: Unit Recompute Amortization (Log-Log)
    # We use Darwin as the reference for deterministic algorithmic behavior
    platforms = ['Darwin', 'Linux', 'Windows']
    colors = {'Needleman-Wunsch': '#1f77b4', 'Forward': '#2ca02c', 'Graph-DP': '#d62728'}
    markers = {'Needleman-Wunsch': 'o', 'Forward': 's', 'Graph-DP': '^'}
    
    for alg in colors.keys():
        alg_df = df[(df['algorithm'] == alg) & (df['platform'] == 'Darwin')].sort_values('granularity_unit_size')
        if not alg_df.empty:
            ax1.plot(alg_df['granularity_unit_size'], alg_df['unit_recompute_events'], 
                     marker=markers[alg], color=colors[alg], linewidth=2.5, markersize=10, 
                     label=f'{alg} (Empirical)')

    # Add Theoretical 1/G Slopes for context
    g_fine = np.logspace(0, 2, 100)
    ax1.plot(g_fine, 324900/g_fine, '--', color='#7f8c8d', alpha=0.5, label='Theoretical Ideal (1/G)')
    
    ax1.set_xscale('log')
    ax1.set_yscale('log')
    ax1.set_xlabel('Granularity Unit Size (G)', fontsize=14, fontweight='bold')
    ax1.set_ylabel('Unit Recompute Events (Log)', fontsize=14, fontweight='bold')
    ax1.set_title('A: Algorithmic Management Amortization', fontsize=16, fontweight='bold', pad=20)
    ax1.legend(frameon=True, shadow=True)
    ax1.grid(True, which="both", ls="-", alpha=0.2)

    # 2. Subplot B: Cross-Platform Performance Scale (Log Runtime)
    # This shows the absolute execution floor across OSs for G=Max
    algs = ['Needleman-Wunsch', 'Forward', 'Graph-DP']
    x = np.arange(len(algs))
    width = 0.25
    
    os_colors = {'Darwin': '#3498db', 'Linux': '#2ecc71', 'Windows': '#e74c3c'}
    
    for i, plat in enumerate(platforms):
        runtimes = []
        for alg in algs:
            # We take the runtime at the largest G tested for that algorithm
            a_df = df[(df['platform'] == plat) & (df['algorithm'] == alg)]
            if not a_df.empty:
                runtimes.append(a_df['runtime_us'].min())
            else:
                runtimes.append(1) # Fallback
        
        ax2.bar(x + (i - 1) * width, runtimes, width, label=plat, 
                color=os_colors[plat], edgecolor='black', alpha=0.8)

    ax2.set_yscale('log') # Use Log scale to see deltas between 50us and 3000us clearly
    ax2.set_ylabel('Execution Time (µs, Log Scale)', fontsize=14, fontweight='bold')
    ax2.set_xticks(x)
    ax2.set_xticklabels(algs, fontsize=12, fontweight='bold')
    ax2.set_title('B: Cross-Platform Performance Floor (G=Max)', fontsize=16, fontweight='bold', pad=20)
    ax2.legend(title='Operating System', frameon=True, shadow=True)
    ax2.grid(axis='y', which='both', linestyle='--', alpha=0.4)

    # Annotate the specific numbers for clarity
    ax2.text(0, 4000, "Quadratic Floor", ha='center', fontsize=10, style='italic')
    ax2.text(1, 200, "Linear Floor", ha='center', fontsize=10, style='italic')
    ax2.text(2, 250, "DAG Floor", ha='center', fontsize=10, style='italic')

    plt.tight_layout(rect=[0, 0.03, 1, 0.95])
    plt.savefig('docs/phase_5_global_master.png', dpi=300, bbox_inches='tight')
    print("Master plot updated: docs/phase_5_global_master.png")

if __name__ == "__main__":
    generate_phase5_master()
