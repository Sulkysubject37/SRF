import pandas as pd
import matplotlib.pyplot as plt
import os
import numpy as np

def generate_phase4_plots(csv_path, output_dir):
    if not os.path.exists(csv_path):
        return

    try:
        df = pd.read_csv(csv_path)
    except:
        return

    if df.empty:
        return

    os.makedirs(output_dir, exist_ok=True)
    plt.style.use('ggplot')

    # 1. Runtime vs Backend (Per Algorithm)
    for alg in df['algorithm'].unique():
        alg_df = df[df['algorithm'] == alg]
        max_size = alg_df['param_2'].max()
        scale_df = alg_df[alg_df['param_2'] == max_size]
        
        plt.figure(figsize=(10, 6))
        backends = scale_df['backend_type'].unique()
        runtimes = [scale_df[scale_df['backend_type'] == b]['runtime_us'].iloc[0] for b in backends]
        
        plt.bar(backends, runtimes, color=['steelblue', 'indianred'])
        plt.ylabel('Runtime (µs)')
        plt.title(f'{alg} Runtime: CPU vs GPU (Size={max_size})')
        plt.savefig(os.path.join(output_dir, f"{alg.lower().replace(' ', '_')}_backend_comp.png"), dpi=300)
        plt.close()

    # 2. Runtime vs Input Size (Backend-aware)
    for alg in df['algorithm'].unique():
        alg_df = df[df['algorithm'] == alg]
        plt.figure(figsize=(10, 6))
        for b in alg_df['backend_type'].unique():
            b_df = alg_df[alg_df['backend_type'] == b].sort_values('param_2')
            plt.plot(b_df['param_2'], b_df['runtime_us'], marker='o', label=f'Backend: {b}')
        
        plt.xlabel('Input Size (SeqLen / Nodes)')
        plt.ylabel('Runtime (µs)')
        plt.title(f'{alg} Scalability: Runtime vs Input Size')
        plt.legend()
        plt.savefig(os.path.join(output_dir, f"{alg.lower().replace(' ', '_')}_scalability.png"), dpi=300)
        plt.close()

    # 3. Transfer Overhead vs Input Size (GPU only)
    gpu_df = df[df['backend_type'] == 'gpu']
    if not gpu_df.empty:
        plt.figure(figsize=(10, 6))
        for alg in gpu_df['algorithm'].unique():
            alg_gpu = gpu_df[gpu_df['algorithm'] == alg].sort_values('param_2')
            # Handle NA or missing transfer overhead
            if 'transfer_overhead_us' in alg_gpu.columns:
                valid_transfer = alg_gpu[alg_gpu['transfer_overhead_us'] != 'NA']
                if not valid_transfer.empty:
                    plt.plot(valid_transfer['param_2'], valid_transfer['transfer_overhead_us'].astype(float), marker='s', label=alg)
        
        plt.xlabel('Input Size')
        plt.ylabel('Simulated Transfer Overhead (µs)')
        plt.title('Phase 4: GPU Data Transfer Scalability')
        plt.legend()
        plt.savefig(os.path.join(output_dir, "gpu_transfer_overhead.png"), dpi=300)
        plt.close()

if __name__ == "__main__":
    generate_phase4_plots("results/csv/benchmark_log.csv", "results/plots")
