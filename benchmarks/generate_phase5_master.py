import pandas as pd
import matplotlib.pyplot as plt
import numpy as np

def generate_phase5_master():
    df = pd.read_csv('docs/phase_5_results_consolidated.csv')
    plt.style.use('bmh')
    fig, (ax1, ax2) = plt.subplots(1, 2, figsize=(20, 10))
    fig.suptitle('Phase 5-A: Granularity & Overhead Amortization Study', fontsize=22, fontweight='bold')

    # 1. Event Amortization: Unit Recomputes vs Granularity (Log-Log)
    # We'll use Darwin data as a proxy for deterministic event behavior
    nw_darwin = df[(df['algorithm'] == 'Needleman-Wunsch') & (df['platform'] == 'Darwin')].sort_values('granularity_unit_size')
    f_darwin = df[(df['algorithm'] == 'Forward') & (df['platform'] == 'Darwin')].sort_values('granularity_unit_size')
    g_darwin = df[(df['algorithm'] == 'Graph-DP') & (df['platform'] == 'Darwin')].sort_values('granularity_unit_size')

    ax1.plot(nw_darwin['granularity_unit_size'], nw_darwin['unit_recompute_events'], marker='o', linewidth=2, label='NW Tiles')
    ax1.plot(f_darwin['granularity_unit_size'], f_darwin['unit_recompute_events'], marker='s', linewidth=2, label='Forward Segments')
    ax1.plot(g_darwin['granularity_unit_size'], g_darwin['unit_recompute_events'], marker='^', linewidth=2, label='Graph Groups')
    
    ax1.set_xscale('log')
    ax1.set_yscale('log')
    ax1.set_xlabel('Granularity Unit Size (G)', fontsize=14)
    ax1.set_ylabel('Unit Recompute Events', fontsize=14)
    ax1.set_title('Algorithmic Amortization (Log-Log Scale)', fontsize=16, fontweight='bold')
    ax1.legend()
    ax1.grid(True, which="both", ls="-", alpha=0.5)

    # 2. Runtime Delta: Granular (G>1) vs Non-Granular (G=1)
    # Let's compare the G=1 vs Max G for each algorithm on Ubuntu (Linux)
    linux_df = df[df['platform'] == 'Linux']
    algs = ['Needleman-Wunsch', 'Forward', 'Graph-DP']
    platforms = ['Darwin', 'Linux', 'Windows']
    
    # We want to show how much "management noise" exists. 
    # For many algorithms, runtime is flat because the compute core dominates.
    # But for Graph-DP, the delta is more visible.
    
    index = np.arange(len(algs))
    bar_width = 0.35
    
    g1_runtimes = []
    gmax_runtimes = []
    
    for alg in algs:
        a_df = linux_df[linux_df['algorithm'] == alg]
        g1_runtimes.append(a_df[a_df['granularity_unit_size'] == 1]['runtime_us'].values[0])
        gmax_runtimes.append(a_df['runtime_us'].min()) # Best granular case
        
    ax2.bar(index, g1_runtimes, bar_width, label='Non-Granular (G=1)', color='steelblue', alpha=0.7)
    ax2.bar(index + bar_width, gmax_runtimes, bar_width, label='Granular (Best G)', color='seagreen', alpha=0.7)
    
    ax2.set_title('Runtime Impact: Linux (µs)', fontsize=16, fontweight='bold')
    ax2.set_xticks(index + bar_width / 2)
    ax2.set_xticklabels(algs)
    ax2.set_ylabel('Runtime (µs)')
    ax2.legend()

    plt.tight_layout(rect=[0, 0.03, 1, 0.95])
    plt.savefig('docs/phase_5_global_master.png', dpi=300)
    print("Phase 5 Global Master Plot saved to docs/phase_5_global_master.png")

if __name__ == "__main__":
    generate_phase5_master()
