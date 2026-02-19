import pandas as pd
import matplotlib.pyplot as plt
import os
import numpy as np

def generate_phase5a_plots(csv_path, output_dir):
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

    # 1. Runtime vs Granularity Unit Size
    for alg in df['algorithm'].unique():
        alg_df = df[df['algorithm'] == alg]
        plt.figure(figsize=(10, 6))
        for b in alg_df['backend_type'].unique():
            b_df = alg_df[alg_df['backend_type'] == b].sort_values('granularity_unit_size')
            if not b_df.empty:
                plt.plot(b_df['granularity_unit_size'], b_df['runtime_us'], marker='o', label=f'Backend: {b}')
        
        plt.xlabel('Granularity Unit Size (Tile/Segment/Group)')
        plt.ylabel('Runtime (µs)')
        plt.title(f'{alg}: Runtime vs Granularity Unit Size')
        plt.legend()
        plt.savefig(os.path.join(output_dir, f"{alg.lower().replace(' ', '_')}_runtime_vs_granularity.png"), dpi=300)
        plt.close()

    # 2. Unit Recompute Events vs Granularity
    for alg in df['algorithm'].unique():
        alg_df = df[df['algorithm'] == alg]
        plt.figure(figsize=(10, 6))
        # Backend doesn't affect deterministic event counts, so we just take CPU for simplicity
        b_df = alg_df[alg_df['backend_type'] == 'cpu'].sort_values('granularity_unit_size')
        if not b_df.empty:
            plt.plot(b_df['granularity_unit_size'], b_df['unit_recompute_events'], marker='s', color='orange', label='Unit Recompute Events')
            plt.plot(b_df['granularity_unit_size'], b_df['recompute_events'], marker='x', linestyle='--', color='gray', label='Cell/Step Recompute Events (Base)')
        
        plt.xlabel('Granularity Unit Size')
        plt.ylabel('Event Count')
        plt.title(f'{alg}: Recomputation Events vs Granularity')
        plt.legend()
        plt.savefig(os.path.join(output_dir, f"{alg.lower().replace(' ', '_')}_events_vs_granularity.png"), dpi=300)
        plt.close()

    # 3. Unit Reuse Proxy vs Granularity
    for alg in df['algorithm'].unique():
        alg_df = df[df['algorithm'] == alg]
        plt.figure(figsize=(10, 6))
        b_df = alg_df[alg_df['backend_type'] == 'cpu'].sort_values('granularity_unit_size')
        if not b_df.empty:
            plt.plot(b_df['granularity_unit_size'], b_df['unit_reuse_proxy'], marker='^', color='green', label='Unit Reuse Proxy')
        
        plt.xlabel('Granularity Unit Size')
        plt.ylabel('Reuse Count')
        plt.title(f'{alg}: Unit Reuse Proxy vs Granularity')
        plt.legend()
        plt.savefig(os.path.join(output_dir, f"{alg.lower().replace(' ', '_')}_reuse_vs_granularity.png"), dpi=300)
        plt.close()

if __name__ == "__main__":
    generate_phase5a_plots("results/csv/benchmark_log.csv", "results/plots")