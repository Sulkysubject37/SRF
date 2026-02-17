import pandas as pd
import matplotlib.pyplot as plt
import os
import sys

def plot_results(csv_path, output_dir):
    if not os.path.exists(csv_path):
        print(f"Error: {csv_path} not found.")
        return

    try:
        df = pd.read_csv(csv_path)
    except Exception as e:
        print(f"Error reading CSV: {e}")
        return

    if df.empty:
        print("CSV is empty, skipping plot.")
        return

    # Use a neutral style
    plt.style.use('ggplot')
    
    plt.figure(figsize=(10, 6))
    
    # Filter out any failed runs if status column exists
    if 'Status' in df.columns:
        df = df[df['Status'] == 'Success']

    if df.empty:
        print("No successful runs to plot.")
        return

    # Plotting Time_us vs Algorithm
    plt.bar(df['Algorithm'], df['Time_us'], color='skyblue', edgecolor='navy')
    
    plt.xlabel('Algorithm')
    plt.ylabel('Execution Time (microseconds)')
    plt.title(f"SRF Baseline Performance - {df['Platform'].iloc[0]}")
    plt.xticks(rotation=45, ha='right')
    plt.tight_layout()

    os.makedirs(output_dir, exist_ok=True)
    platform_name = df['Platform'].iloc[0].lower()
    output_path = os.path.join(output_dir, f"baseline_performance_{platform_name}.png")
    
    plt.savefig(output_path)
    print(f"Plot saved to {output_path}")

if __name__ == "__main__":
    csv_file = "results/csv/benchmark_log.csv"
    plot_dir = "results/plots"
    plot_results(csv_file, plot_dir)
