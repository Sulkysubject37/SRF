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

    df = df[df['Status'] == 'Success'].sort_values(by='Algorithm')
    os.makedirs(output_dir, exist_ok=True)
    platform = df['Platform'].iloc[0].replace(' ', '_').lower()

    # 1. Runtime Plot
    plt.style.use('bmh')
    fig, ax = plt.subplots(figsize=(10, 6))
    bars = ax.bar(df['Algorithm'], df['Time_us'], color='steelblue', alpha=0.8)
    ax.set_ylabel('Execution Time (µs)')
    ax.set_title(f'Runtime Baseline - {df["Platform"].iloc[0]}')
    plt.xticks(rotation=30, ha='right')
    plt.tight_layout()
    plt.savefig(os.path.join(output_dir, f"runtime_{platform}.png"), dpi=300)
    plt.close()

    # 2. Memory Plot
    fig, ax = plt.subplots(figsize=(10, 6))
    bars = ax.bar(df['Algorithm'], df['Memory_kb'], color='indianred', alpha=0.8)
    ax.set_ylabel('Peak Memory Usage (KB)')
    ax.set_title(f'Memory Baseline - {df["Platform"].iloc[0]}')
    plt.xticks(rotation=30, ha='right')
    plt.tight_layout()
    plt.savefig(os.path.join(output_dir, f"memory_{platform}.png"), dpi=300)
    plt.close()

if __name__ == "__main__":
    plot_results("results/csv/benchmark_log.csv", "results/plots")
