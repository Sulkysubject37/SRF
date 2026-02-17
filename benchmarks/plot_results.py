import pandas as pd
import matplotlib.pyplot as plt
import os

def plot_results(csv_path, output_dir):
    if not os.path.exists(csv_path):
        return

    try:
        df = pd.read_csv(csv_path)
    except:
        return

    if df.empty:
        return

    # Filter successful runs and sort for consistency
    df = df[df['Status'] == 'Success'].sort_values(by='Algorithm')
    os.makedirs(output_dir, exist_ok=True)
    
    # Identify unique platforms in the data
    platforms = df['Platform'].unique()

    for platform_raw in platforms:
        platform_df = df[df['Platform'] == platform_raw]
        platform_clean = platform_raw.replace(' ', '_').lower()

        # Create a Master Plot with 3 subplots
        plt.style.use('bmh')
        fig, (ax1, ax2, ax3) = plt.subplots(1, 3, figsize=(18, 6))
        
        fig.suptitle(f'SRF Phase 1 Master Performance Profile: {platform_raw}', fontsize=16, fontweight='bold')

        # 1. Runtime Subplot (µs)
        ax1.bar(platform_df['Algorithm'], platform_df['Time_us'], color='steelblue', alpha=0.8, edgecolor='black')
        ax1.set_title('Average Runtime', fontsize=12)
        ax1.set_ylabel('Time (µs)')
        ax1.tick_params(axis='x', rotation=45)

        # 2. Memory Subplot (KB)
        ax2.bar(platform_df['Algorithm'], platform_df['Memory_kb'], color='indianred', alpha=0.8, edgecolor='black')
        ax2.set_title('Peak Memory Usage', fontsize=12)
        ax2.set_ylabel('Memory (KB)')
        ax2.tick_params(axis='x', rotation=45)

        # 3. Cache Diagnostic Subplot (Accesses)
        ax3.bar(platform_df['Algorithm'], platform_df['Cache_Hits_Diagnostic'], color='seagreen', alpha=0.8, edgecolor='black')
        ax3.set_title('Cache Access Diagnostic', fontsize=12)
        ax3.set_ylabel('Total DP Cell Accesses')
        ax3.tick_params(axis='x', rotation=45)

        plt.tight_layout(rect=[0, 0.03, 1, 0.95])
        
        master_plot_path = os.path.join(output_dir, f"master_profile_{platform_clean}.png")
        plt.savefig(master_plot_path, dpi=300)
        print(f"Master plot saved for {platform_raw} at {master_plot_path}")
        plt.close()

if __name__ == "__main__":
    plot_results("results/csv/benchmark_log.csv", "results/plots")