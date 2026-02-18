import pandas as pd
import matplotlib.pyplot as plt
import numpy as np

def generate_phase4_master():
    df = pd.read_csv('docs/phase_4_results_consolidated.csv')
    
    plt.style.use('bmh')
    fig, ((ax1, ax2), (ax3, ax4)) = plt.subplots(2, 2, figsize=(20, 14))
    fig.suptitle('Phase 4: Global Scalability & Multi-Backend Study (Final)', fontsize=22, fontweight='bold')

    # 1. Scalability: Needleman-Wunsch (Runtime vs Input Size)
    nw_df = df[df['algorithm'] == 'Needleman-Wunsch']
    platforms = ['Darwin', 'Linux', 'Windows']
    colors = {'Darwin': '#1f77b4', 'Linux': '#2ca02c', 'Windows': '#d62728'}
    
    for plat in platforms:
        p_df = nw_df[(nw_df['platform'] == plat) & (nw_df['backend_type'] == 'cpu')].sort_values('param_2')
        # We only have one point for size 1000 in this consolidated file, 
        # but let's assume we want to show the cross-platform comparison at max scale
        pass

    # Actually, let's make Subplot 1: Backend Comparison at Max Scale
    index = np.arange(len(platforms))
    bar_width = 0.35
    
    nw_1000 = nw_df[nw_df['param_2'] == 1000]
    cpu_runtimes = [nw_1000[(nw_1000['platform'] == p) & (nw_1000['backend_type'] == 'cpu')]['runtime_us'].values[0] for p in platforms]
    gpu_runtimes = [nw_1000[(nw_1000['platform'] == p) & (nw_1000['backend_type'] == 'gpu')]['runtime_us'].values[0] for p in platforms]
    
    ax1.bar(index, cpu_runtimes, bar_width, label='CPU Backend', color='steelblue', edgecolor='black')
    ax1.bar(index + bar_width, gpu_runtimes, bar_width, label='GPU (Simulated)', color='indianred', edgecolor='black')
    ax1.set_title('NW 1000: Backend Comparison (µs)', fontsize=16, fontweight='bold')
    ax1.set_xticks(index + bar_width/2)
    ax1.set_xticklabels(platforms)
    ax1.legend()

    # 2. Forward 5000: Cross-Platform Performance
    f_5000 = df[(df['algorithm'] == 'Forward') & (df['param_2'] == 5000) & (df['backend_type'] == 'cpu')]
    ax2.bar(f_5000['platform'], f_5000['runtime_us'], color='seagreen', alpha=0.8, edgecolor='black')
    ax2.set_title('Forward 5000: CPU Runtime Across OS (µs)', fontsize=16, fontweight='bold')
    ax2.set_ylabel('Execution Time (µs)')

    # 3. Graph-DP 10000: Efficiency at Scale
    g_10000 = df[(df['algorithm'] == 'Graph-DP') & (df['param_2'] == 10000) & (df['backend_type'] == 'cpu')]
    ax3.bar(g_10000['platform'], g_10000['runtime_us'], color='purple', alpha=0.8, edgecolor='black')
    ax3.set_title('Graph-DP 10000: Runtime Efficiency (µs)', fontsize=16, fontweight='bold')
    ax3.set_ylabel('Execution Time (µs)')

    # 4. Memory Footprint: Constant O(N) Baseline
    # Show memory usage for NW 1000 across platforms
    mem_vals = [nw_1000[nw_1000['platform'] == p]['peak_memory_kb'].mean() for p in platforms]
    ax4.bar(platforms, mem_vals, color='orange', alpha=0.8, edgecolor='black')
    ax4.set_title('Peak Memory Usage (KB) at Max Scale', fontsize=16, fontweight='bold')
    ax4.set_ylabel('Memory (KB)')

    plt.tight_layout(rect=[0, 0.03, 1, 0.95])
    plt.savefig('docs/phase_4_global_master.png', dpi=300)
    print("Phase 4 Global Master Plot saved to docs/phase_4_global_master.png")

if __name__ == "__main__":
    generate_phase4_master()
