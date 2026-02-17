import pandas as pd
import matplotlib.pyplot as plt
import numpy as np
import os

def generate_global_master_plot():
    df = pd.read_csv('docs/phase_1_baselines.csv')
    
    algorithms = sorted(df['Algorithm'].unique())
    platforms = ['Darwin', 'Linux', 'Windows']
    
    plt.style.use('bmh')
    fig, (ax1, ax2, ax3) = plt.subplots(1, 3, figsize=(20, 7))
    fig.suptitle('Phase 1: Global Cross-Platform Baseline Characterization', fontsize=18, fontweight='bold')

    bar_width = 0.25
    index = np.arange(len(algorithms))
    colors = {'Darwin': 'royalblue', 'Linux': 'seagreen', 'Windows': 'indianred'}

    for i, platform in enumerate(platforms):
        p_df = df[df['Platform'] == platform].sort_values('Algorithm')
        
        # Subplot 1: Runtime (Log Scale for visibility)
        ax1.bar(index + i * bar_width, p_df['Time_us'], bar_width, label=platform, 
                color=colors[platform], edgecolor='black', alpha=0.8)
        
        # Subplot 2: Memory
        ax2.bar(index + i * bar_width, p_df['Memory_kb'], bar_width, label=platform, 
                color=colors[platform], edgecolor='black', alpha=0.8)
        
        # Subplot 3: Cache Diagnostic
        ax3.bar(index + i * bar_width, p_df['Cache_Hits_Diagnostic'], bar_width, label=platform, 
                color=colors[platform], edgecolor='black', alpha=0.8)

    # Styling ax1 (Runtime)
    ax1.set_title('Average Runtime', fontsize=14, fontweight='bold')
    ax1.set_ylabel('Time (µs) - Log Scale')
    ax1.set_yscale('log')
    ax1.set_xticks(index + bar_width)
    ax1.set_xticklabels(algorithms, rotation=30)
    ax1.legend()

    # Styling ax2 (Memory)
    ax2.set_title('Peak Memory Usage', fontsize=14, fontweight='bold')
    ax2.set_ylabel('Memory (KB)')
    ax2.set_xticks(index + bar_width)
    ax2.set_xticklabels(algorithms, rotation=30)

    # Styling ax3 (Cache)
    ax3.set_title('Cache Access Diagnostic', fontsize=14, fontweight='bold')
    ax3.set_ylabel('Total Accesses')
    ax3.set_xticks(index + bar_width)
    ax3.set_xticklabels(algorithms, rotation=30)

    plt.tight_layout(rect=[0, 0.03, 1, 0.95])
    
    output_path = 'docs/phase_1_global_master_profile.png'
    plt.savefig(output_path, dpi=300)
    print(f"Global master plot saved to {output_path}")

if __name__ == "__main__":
    generate_global_master_plot()