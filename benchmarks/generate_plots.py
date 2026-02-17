import pandas as pd
import matplotlib.pyplot as plt
import os

def generate_plots(csv_path, output_dir):
    if not os.path.exists(csv_path):
        return

    try:
        df = pd.read_csv(csv_path)
    except:
        return

    if df.empty:
        return

    os.makedirs(output_dir, exist_ok=True)
    platform = df['platform'].iloc[0]

    plt.style.use('ggplot')

    # One plot per algorithm per metric to keep things readable
    for algorithm in df['algorithm'].unique():
        alg_df = df[df['algorithm'] == algorithm]
        
        # 1. Runtime vs Parameter
        plt.figure(figsize=(10, 6))
        for variant in alg_df['variant'].unique():
            var_df = alg_df[alg_df['variant'] == variant]
            # Use largest parameter_2 (sequence length / nodes)
            max_p2 = var_df['parameter_2'].max()
            p2_df = var_df[var_df['parameter_2'] == max_p2]
            plt.plot(p2_df['parameter_1'], p2_df['runtime_us'], marker='o', label=f'{variant} (Size={max_p2})')
        
        plt.xlabel('SRF Control Parameter (TileSize / K-Interval / Depth)')
        plt.ylabel('Runtime (µs)')
        plt.title(f'{algorithm} - Runtime vs Parameter ({platform})')
        plt.legend()
        plt.savefig(os.path.join(output_dir, f"{algorithm.replace(' ', '_').lower()}_runtime.png"), dpi=300)
        plt.close()

        # 2. Memory vs Parameter
        plt.figure(figsize=(10, 6))
        for variant in alg_df['variant'].unique():
            var_df = alg_df[alg_df['variant'] == variant]
            max_p2 = var_df['parameter_2'].max()
            p2_df = var_df[var_df['parameter_2'] == max_p2]
            plt.plot(p2_df['parameter_1'], p2_df['peak_memory_kb'], marker='s', linestyle='--', label=f'{variant}')
        
        plt.xlabel('SRF Control Parameter')
        plt.ylabel('Memory (KB)')
        plt.title(f'{algorithm} - Memory vs Parameter')
        plt.legend()
        plt.savefig(os.path.join(output_dir, f"{algorithm.replace(' ', '_').lower()}_memory.png"), dpi=300)
        plt.close()

        # 3. Recomputation vs Parameter
        srf_df = alg_df[alg_df['variant'].str.contains('SRF')]
        if not srf_df.empty:
            plt.figure(figsize=(10, 6))
            for variant in srf_df['variant'].unique():
                var_df = srf_df[srf_df['variant'] == variant]
                max_p2 = var_df['parameter_2'].max()
                p2_df = var_df[var_df['parameter_2'] == max_p2]
                plt.plot(p2_df['parameter_1'], p2_df['recompute_events'], marker='^', label=f'{variant}')
            
            plt.xlabel('SRF Control Parameter')
            plt.ylabel('Recomputation Events')
            plt.title(f'{algorithm} - Recomputation Overhead')
            plt.legend()
            plt.savefig(os.path.join(output_dir, f"{algorithm.replace(' ', '_').lower()}_recompute.png"), dpi=300)
            plt.close()

if __name__ == "__main__":
    generate_plots("results/csv/benchmark_log.csv", "results/plots")
