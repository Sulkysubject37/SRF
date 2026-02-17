import pandas as pd
import matplotlib.pyplot as plt
import os

def generate_phase3_plots(csv_path, output_dir):
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
    # Clear old plots to avoid confusion
    for f in os.listdir(output_dir):
        if f.endswith(".png"):
            os.remove(os.path.join(output_dir, f))

    plt.style.use('ggplot')

    # 1. Runtime vs Cache Budget (Needleman-Wunsch)
    nw_df = df[df['algorithm'] == 'Needleman-Wunsch']
    if not nw_df.empty:
        plt.figure(figsize=(10, 6))
        srf_nw = nw_df[nw_df['variant'].str.contains('SRF')]
        if not srf_nw.empty:
            plt.plot(srf_nw['cache_budget'], srf_nw['runtime_us'], marker='o', label='SRF-CacheAware')
        
        base_df = nw_df[nw_df['variant'] == 'Baseline']
        if not base_df.empty:
            base_rt = base_df['runtime_us'].iloc[0]
            plt.axhline(y=base_rt, color='r', linestyle='--', label='Baseline')
        
        plt.xlabel('Cache Budget (KB)')
        plt.ylabel('Runtime (µs)')
        plt.title('NW Runtime vs Cache Budget (Fixed Memory)')
        plt.legend()
        plt.savefig(os.path.join(output_dir, "nw_runtime_vs_cache.png"), dpi=300)
        plt.close()

    # 2. Runtime vs Working Set
    plt.figure(figsize=(10, 6))
    for alg in df['algorithm'].unique():
        alg_df = df[df['algorithm'] == alg]
        srf_alg = alg_df[alg_df['variant'].str.contains('SRF')]
        if not srf_alg.empty:
            plt.scatter(srf_alg['working_set_proxy'], srf_alg['runtime_us'], label=alg)
    
    plt.xlabel('Working Set Proxy (Bytes)')
    plt.ylabel('Runtime (µs)')
    plt.title('SRF Runtime vs Working Set Size')
    plt.legend()
    plt.savefig(os.path.join(output_dir, "runtime_vs_workingset.png"), dpi=300)
    plt.close()

    # 3. Recomputation vs Cache Budget (Needleman-Wunsch)
    if not nw_df.empty:
        plt.figure(figsize=(10, 6))
        srf_nw = nw_df[nw_df['variant'].str.contains('SRF')]
        if not srf_nw.empty:
            plt.plot(srf_nw['cache_budget'], srf_nw['recompute_events'], marker='^', color='green', label='Recomputation Events')
            plt.xlabel('Cache Budget (KB)')
            plt.ylabel('Recompute Count')
            plt.title('NW Recomputation Overhead vs Cache Budget')
            plt.legend()
            plt.savefig(os.path.join(output_dir, "nw_recompute_vs_cache.png"), dpi=300)
            plt.close()

if __name__ == "__main__":
    generate_phase3_plots("results/csv/benchmark_log.csv", "results/plots")
