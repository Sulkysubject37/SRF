import pandas as pd
import matplotlib.pyplot as plt
import numpy as np

def generate_phase5_master():
    df = pd.read_csv('docs/phase_5_results_consolidated.csv')
    
    # Professional Styling
    plt.rcParams.update({'font.size': 12, 'font.family': 'sans-serif'})
    fig, (ax1, ax2) = plt.subplots(1, 2, figsize=(20, 9))
    fig.patch.set_facecolor('#f8f9fa')
    
    # 1. Algorithmic Amortization (Log-Log)
    # Filter for Darwin as the deterministic representative
    nw = df[(df['algorithm'] == 'Needleman-Wunsch') & (df['platform'] == 'Darwin')].sort_values('granularity_unit_size')
    fwd = df[(df['algorithm'] == 'Forward') & (df['platform'] == 'Darwin')].sort_values('granularity_unit_size')
    gdp = df[(df['algorithm'] == 'Graph-DP') & (df['platform'] == 'Darwin')].sort_values('granularity_unit_size')

    # Plotting with distinct markers and lines
    ax1.plot(nw['granularity_unit_size'], nw['unit_recompute_events'], 'o-', color='#1f77b4', linewidth=2.5, markersize=8, label='NW Tiles (2D)')
    ax1.plot(fwd['granularity_unit_size'], fwd['unit_recompute_events'], 's-', color='#2ca02c', linewidth=2.5, markersize=8, label='Forward Segments (1D)')
    ax1.plot(gdp['granularity_unit_size'], gdp['unit_recompute_events'], '^-', color='#d62728', linewidth=2.5, markersize=8, label='Graph Groups (DAG)')

    # Ideal Theoretical Slope (1/G)
    g_range = np.linspace(1, 100, 100)
    ideal = nw['unit_recompute_events'].iloc[0] / g_range
    ax1.plot(g_range, ideal, '--', color='#7f8c8d', alpha=0.6, label='Theoretical Ideal (1/G)')

    ax1.set_xscale('log')
    ax1.set_yscale('log')
    ax1.set_xlabel('Granularity Unit Size (G)', fontsize=14, fontweight='bold')
    ax1.set_ylabel('Unit Recompute Events', fontsize=14, fontweight='bold')
    ax1.set_title('A: Recomputation Event Amortization', fontsize=16, fontweight='bold', pad=20)
    ax1.legend(frameon=True, facecolor='white', shadow=True)
    ax1.grid(True, which="both", ls="-", alpha=0.3)

    # Annotate Amortization Ratios
    ax1.annotate('38x Reduction', xy=(40, 8550), xytext=(50, 15000), 
                 arrowprops=dict(facecolor='black', shrink=0.05, width=1, headwidth=5))
    ax1.annotate('100x Reduction', xy=(100, 20), xytext=(20, 10), 
                 arrowprops=dict(facecolor='black', shrink=0.05, width=1, headwidth=5))

    # 2. Runtime Overhead Amortization (Bar Chart)
    # Comparing G=1 vs G=Max across Platforms
    algs = ['Needleman-Wunsch', 'Forward', 'Graph-DP']
    platforms = ['Darwin', 'Linux', 'Windows']
    
    # We'll create a grouped bar chart
    x = np.arange(len(algs))
    width = 0.2
    
    colors = {'Darwin': '#3498db', 'Linux': '#2ecc71', 'Windows': '#e74c3c'}
    
    for i, plat in enumerate(platforms):
        p_df = df[df['platform'] == plat]
        # Get Delta (G=1 / G=Max)
        ratios = []
        for alg in algs:
            a_df = p_df[p_df['algorithm'] == alg]
            g1 = a_df[a_df['granularity_unit_size'] == 1]['runtime_us'].iloc[0]
            g_best = a_df['runtime_us'].min()
            # Improvement %
            ratios.append((g1 - g_best) / g1 * 100)
        
        ax2.bar(x + (i - 1) * width, ratios, width, label=plat, color=colors[plat], alpha=0.85, edgecolor='black', linewidth=0.5)

    ax2.set_ylabel('Execution Time Improvement (%)', fontsize=14, fontweight='bold')
    ax2.set_title('B: Management Overhead Reduction (G=1 vs G=Max)', fontsize=16, fontweight='bold', pad=20)
    ax2.set_xticks(x)
    ax2.set_xticklabels(algs, fontsize=12, fontweight='bold')
    ax2.legend(title='Platform', frameon=True, facecolor='white', shadow=True)
    ax2.set_ylim(0, 100)
    ax2.grid(axis='y', linestyle='--', alpha=0.7)

    # Adding a subtle horizontal line for Forward's specific massive improvement
    ax2.axhline(0, color='black', linewidth=0.8)

    plt.tight_layout(rect=[0, 0.03, 1, 0.95])
    plt.savefig('docs/phase_5_global_master.png', dpi=300, bbox_inches='tight')
    print("Publication-quality Phase 5 Global Master Plot saved to docs/phase_5_global_master.png")

if __name__ == "__main__":
    generate_phase5_master()