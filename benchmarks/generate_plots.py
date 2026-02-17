import pandas as pd
import matplotlib.pyplot as plt
import os

def generate_all_plots(csv_path, output_dir):
    if not os.path.exists(csv_path):
        print(f"Error: {csv_path} not found.")
        return

    try:
        df = pd.read_csv(csv_path)
    except Exception as e:
        print(f"Error reading CSV: {e}")
        return

    if df.empty:
        print("CSV is empty.")
        return

    os.makedirs(output_dir, exist_ok=True)
    plt.style.use('ggplot')

    # --- Phase 2 Style Plots: Performance vs Primary Parameter (param_1) ---
    for algorithm in df['algorithm'].unique():
        alg_df = df[df['algorithm'] == algorithm]
        
        # 1. Runtime vs param_1
        plt.figure(figsize=(10, 6))
        for variant in alg_df['variant'].unique():
            var_df = alg_df[alg_df['variant'] == variant]
            # Plot for the largest size (parameter_2) to show trade-offs
            max_p2 = var_df['param_2'].max()
            p2_df = var_df[var_df['param_2'] == max_p2].sort_values('param_1')
            plt.plot(p2_df['param_1'], p2_df['runtime_us'], marker='o', label=f'{variant} (Size={max_p2})')
        
        plt.xlabel('Primary Control Parameter (TileSize / K-Interval / Depth)')
        plt.ylabel('Runtime (µs)')
        plt.title(f'{algorithm} - Runtime vs Parameter')
        plt.legend()
        plt.savefig(os.path.join(output_dir, f"{algorithm.lower().replace(' ', '_')}_runtime_p2.png"), dpi=300)
        plt.close()

        # 2. Memory vs param_1
        plt.figure(figsize=(10, 6))
        for variant in alg_df['variant'].unique():
            var_df = alg_df[alg_df['variant'] == variant]
            max_p2 = var_df['param_2'].max()
            p2_df = var_df[var_df['param_2'] == max_p2].sort_values('param_1')
            plt.plot(p2_df['param_1'], p2_df['peak_memory_kb'], marker='s', linestyle='--', label=f'{variant}')
        
        plt.xlabel('Primary Control Parameter')
        plt.ylabel('Memory (KB)')
        plt.title(f'{algorithm} - Memory vs Parameter')
        plt.legend()
        plt.savefig(os.path.join(output_dir, f"{algorithm.lower().replace(' ', '_')}_memory_p2.png"), dpi=300)
        plt.close()

    # --- Phase 3 Style Plots: Locality & Cache Effects ---

    # 3. Runtime vs Cache Budget (Needleman-Wunsch specific)
    nw_df = df[df['algorithm'] == 'Needleman-Wunsch']
    if not nw_df.empty:
        plt.figure(figsize=(10, 6))
        srf_nw = nw_df[nw_df['variant'].str.contains('SRF')]
        # Filter for runs where cache_budget was actually varied (param_3)
        cache_varied = srf_nw[srf_nw['param_3'] > 0].sort_values('cache_budget')
        if not cache_varied.empty:
            plt.plot(cache_varied['cache_budget'], cache_varied['runtime_us'], marker='o', color='blue', label='SRF-CacheAware')
            
            base_rt = nw_df[nw_df['variant'] == 'Baseline']['runtime_us'].iloc[0]
            plt.axhline(y=base_rt, color='r', linestyle='--', label='Baseline')
            
            plt.xlabel('Cache Budget (KB)')
            plt.ylabel('Runtime (µs)')
            plt.title('NW Runtime vs Cache Budget (Fixed Memory)')
            plt.legend()
            plt.savefig(os.path.join(output_dir, "nw_runtime_vs_cache_p3.png"), dpi=300)
            plt.close()

    # 4. Runtime vs Working Set (Global Scatter)
    plt.figure(figsize=(10, 6))
    for alg in df['algorithm'].unique():
        alg_df = df[df['algorithm'] == alg]
        srf_alg = alg_df[alg_df['variant'].str.contains('SRF')]
        if not srf_alg.empty:
            plt.scatter(srf_alg['working_set_proxy'], srf_alg['runtime_us'], label=alg, alpha=0.7)
    
    plt.xlabel('Working Set Proxy (Bytes)')
    plt.ylabel('Runtime (µs)')
    plt.title('Global SRF Runtime vs Working Set Size (Phase 3)')
    plt.legend()
    plt.savefig(os.path.join(output_dir, "runtime_vs_workingset_p3.png"), dpi=300)
    plt.close()

    # 5. Locality Proxy vs Runtime (Forward/Viterbi/Graph)
    plt.figure(figsize=(10, 6))
    loc_df = df[df['locality_proxy'] > 0]
    if not loc_df.empty:
        for alg in loc_df['algorithm'].unique():
            alg_df = loc_df[loc_df['algorithm'] == alg]
            plt.scatter(alg_df['locality_proxy'], alg_df['runtime_us'], label=alg, marker='x')
        
        plt.xlabel('Locality Proxy (Dependency Distance Sum)')
        plt.ylabel('Runtime (µs)')
        plt.title('Impact of Locality Proxy on SRF Runtime')
        plt.legend()
        plt.savefig(os.path.join(output_dir, "runtime_vs_locality_p3.png"), dpi=300)
        plt.close()

if __name__ == "__main__":
    generate_all_plots("results/csv/benchmark_log.csv", "results/plots")