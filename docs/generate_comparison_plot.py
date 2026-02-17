import pandas as pd
import matplotlib.pyplot as plt
import numpy as np

def generate_plot():
    df = pd.read_csv('docs/phase_1_baselines.csv')
    
    algorithms = df['Algorithm'].unique()
    platforms = df['Platform'].unique()
    
    # Setting up the plot
    plt.style.use('bmh')
    fig, ax = plt.subplots(figsize=(12, 8))
    
    bar_width = 0.2
    index = np.arange(len(algorithms))
    
    colors = {'Darwin': 'royalblue', 'Linux': 'seagreen', 'Windows': 'indianred'}
    
    for i, platform in enumerate(platforms):
        platform_data = df[df['Platform'] == platform].sort_values('Algorithm')
        ax.bar(index + i * bar_width, platform_data['Time_us'], 
               bar_width, label=platform, color=colors.get(platform, 'gray'),
               edgecolor='black', alpha=0.85)

    ax.set_xlabel('Algorithm', fontsize=12, fontweight='bold')
    ax.set_ylabel('Execution Time (µs)', fontsize=12, fontweight='bold')
    ax.set_title('Phase 1: Cross-Platform Baseline Comparison', fontsize=14, fontweight='bold')
    ax.set_xticks(index + bar_width)
    ax.set_xticklabels(sorted(algorithms), fontsize=11)
    ax.legend(title="Platform", fontsize=10)
    
    # Use log scale for better visualization since NW is much larger
    ax.set_yscale('log')
    ax.set_ylabel('Execution Time (µs) - Log Scale', fontsize=12, fontweight='bold')
    
    plt.grid(axis='y', which='both', linestyle='--', alpha=0.5)
    plt.tight_layout()
    
    plt.savefig('docs/phase_1_comparison.png', dpi=300)
    print("Cross-platform comparison plot saved to docs/phase_1_comparison.png")

if __name__ == "__main__":
    generate_plot()
