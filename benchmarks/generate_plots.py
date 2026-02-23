import pandas as pd
import matplotlib.pyplot as plt
import os
import numpy as np

def generate_phase8_plots(df, output_dir):
    if 'R_mem' not in df.columns or 'R_rec' not in df.columns:
        return

    fig, (ax1, ax2) = plt.subplots(1, 2, figsize=(20, 9))
    fig.patch.set_facecolor('#ffffff')

    platforms = df['platform'].unique()
    algs = df['algorithm'].unique()
    
    x = np.arange(len(algs))
    width = 0.8 / len(platforms)

    for i, plat in enumerate(platforms):
        p_df = df[df['platform'] == plat]
        r_mems = []
        r_recs = []
        for a in algs:
            a_df = p_df[p_df['algorithm'] == a]
            if not a_df.empty:
                val_mem = a_df['R_mem'].iloc[-1]
                val_rec = a_df['R_rec'].iloc[-1]
                try:
                    r_mems.append(float(val_mem) if val_mem != 'NA' else 0)
                except:
                    r_mems.append(0)
                try:
                    r_recs.append(float(val_rec) if val_rec != 'NA' else 0)
                except:
                    r_recs.append(0)
            else:
                r_mems.append(0)
                r_recs.append(0)
        
        ax1.bar(x + (i - (len(platforms)-1)/2) * width, r_mems, width, label=f'{plat} R_mem')
        ax2.bar(x + (i - (len(platforms)-1)/2) * width, r_recs, width, label=f'{plat} R_rec')

    ax1.set_ylabel('Memory Access Ratio (R_mem)', fontsize=14, fontweight='bold')
    ax1.set_title('A: Execution Memory Intensity', fontsize=16, fontweight='bold')
    ax1.set_xticks(x)
    ax1.set_xticklabels(algs)
    ax1.legend()

    ax2.set_ylabel('Recomputation Ratio (R_rec)', fontsize=14, fontweight='bold')
    ax2.set_title('B: Recomputation Arithmetic Intensity', fontsize=16, fontweight='bold')
    ax2.set_xticks(x)
    ax2.set_xticklabels(algs)
    ax2.legend()

    plt.tight_layout()
    plot_path = os.path.join(output_dir, "phase_8_regime_stability.png")
    plt.savefig(plot_path, dpi=300)
    plt.close()

def generate_all_plots(csv_path, output_dir):
    if not os.path.exists(csv_path): return
    try:
        df = pd.read_csv(csv_path)
    except: return
    if df.empty: return

    os.makedirs(output_dir, exist_ok=True)
    plt.style.use('ggplot')

    # Basic Scaling Plot
    for alg in df['algorithm'].unique():
        alg_df = df[df['algorithm'] == alg].sort_values('input_size')
        if not alg_df.empty and alg_df['input_size'].max() > 0:
            plt.figure(figsize=(10, 6))
            for plat in alg_df['platform'].unique():
                p_df = alg_df[alg_df['platform'] == plat]
                plt.plot(p_df['input_size'], p_df['runtime_us'], marker='o', label=plat)
            plt.yscale('log')
            plt.title(f'{alg} Scaling')
            plt.legend()
            plt.savefig(os.path.join(output_dir, f"{alg.lower()}_scaling.png"))
            plt.close()

    # Phase 8 Plots
    generate_phase8_plots(df, output_dir)

if __name__ == "__main__":
    generate_all_plots("results/csv/benchmark_log.csv", "results/plots")
