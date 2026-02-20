import pandas as pd
import matplotlib.pyplot as plt
import os

def main():
    csv_path = "results/csv/regime_mapping.csv"
    output_dir = "results/plots"
    if not os.path.exists(csv_path): return
    
    df = pd.read_csv(csv_path)
    plt.style.use('seaborn-v0_8-whitegrid')
    
    # 1. Extreme Runtime Scaling
    plt.figure(figsize=(12, 7))
    for alg in df['algorithm'].unique():
        a_df = df[df['algorithm'] == alg].sort_values('input_size')
        plt.plot(a_df['input_size'], a_df['runtime_us'], marker='o', label=alg, linewidth=2)
    
    plt.xscale('log')
    plt.yscale('log')
    plt.xlabel('Input Size (Length / Nodes)', fontweight='bold')
    plt.ylabel('Runtime (µs)', fontweight='bold')
    plt.title('Extreme-Scale Runtime Characterization (XS to 1M)', fontsize=14, fontweight='bold')
    plt.legend()
    plt.savefig(os.path.join(output_dir, "extreme_runtime_scaling.png"), dpi=300)
    plt.close()

    # 2. Extreme Recomputation Load
    plt.figure(figsize=(12, 7))
    for alg in df['algorithm'].unique():
        a_df = df[df['algorithm'] == alg].sort_values('input_size')
        plt.plot(a_df['input_size'], a_df['recompute_events'], marker='s', label=alg, linewidth=2)
    
    plt.xscale('log')
    plt.yscale('log')
    plt.xlabel('Input Size', fontweight='bold')
    plt.ylabel('Recomputation Events', fontweight='bold')
    plt.title('Extreme-Scale Recomputation Load', fontsize=14, fontweight='bold')
    plt.legend()
    plt.savefig(os.path.join(output_dir, "extreme_recompute_scaling.png"), dpi=300)
    plt.close()

if __name__ == "__main__":
    main()
