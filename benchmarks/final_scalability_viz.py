import pandas as pd
import matplotlib.pyplot as plt
import os

def generate_final_scalability():
    df = pd.read_csv('results/csv/benchmark_log.csv')
    plt.style.use('bmh')
    
    fig, (ax1, ax2) = plt.subplots(1, 2, figsize=(18, 8))
    fig.suptitle('SRF Final Scalability Analysis: Complexity vs Architecture', fontsize=18, fontweight='bold')

    # 1. Needleman-Wunsch Quadratic Scaling (CPU vs GPU)
    nw = df[df['algorithm'] == 'Needleman-Wunsch']
    for b in nw['backend_type'].unique():
        b_df = nw[nw['backend_type'] == b].sort_values('param_2')
        ax1.plot(b_df['param_2'], b_df['runtime_us'], marker='o', label=f'NW ({b})')
    
    ax1.set_xlabel('Sequence Length (N)')
    ax1.set_ylabel('Runtime (µs)')
    ax1.set_title('Quadratic Scaling Profile (Alignment)', fontsize=14)
    ax1.legend()

    # 2. Linear & Log-Linear Scaling (HMM & Graph)
    others = df[df['algorithm'] != 'Needleman-Wunsch']
    for alg in others['algorithm'].unique():
        alg_df = others[(others['algorithm'] == alg) & (others['backend_type'] == 'cpu')].sort_values('param_2')
        ax2.plot(alg_df['param_2'], alg_df['runtime_us'], marker='s', label=f'{alg} (cpu)')
    
    ax2.set_xlabel('Input Size (Steps / Nodes)')
    ax2.set_ylabel('Runtime (µs)')
    ax2.set_title('Linear & Log-Linear Efficiency', fontsize=14)
    ax2.legend()

    plt.tight_layout(rect=[0, 0.03, 1, 0.95])
    output_path = 'results/plots/final_scalability_study.png'
    plt.savefig(output_path, dpi=300)
    print(f"Final scalability plot saved to {output_path}")

if __name__ == "__main__":
    generate_final_scalability()
