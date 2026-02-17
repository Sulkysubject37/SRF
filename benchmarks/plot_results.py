import pandas as pd
import matplotlib.pyplot as plt
import os

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

    # Filter out any failed runs
    if 'Status' in df.columns:
        df = df[df['Status'] == 'Success']

    if df.empty:
        print("No successful runs to plot.")
        return

    # Sort algorithms alphabetically for consistent ordering
    df = df.sort_values(by='Algorithm')

    # Use a clean style
    plt.style.use('bmh')
    
    fig, ax = plt.subplots(figsize=(12, 7))
    
    # Plotting Time_us vs Algorithm
    bars = ax.bar(df['Algorithm'], df['Time_us'], color='steelblue', edgecolor='black', alpha=0.8)
    
    # Add values on top of bars
    for bar in bars:
        height = bar.get_height()
        ax.annotate(f'{height:.2f}',
                    xy=(bar.get_x() + bar.get_width() / 2, height),
                    xytext=(0, 3),  # 3 points vertical offset
                    textcoords="offset points",
                    ha='center', va='bottom', fontsize=10)

    ax.set_xlabel('Algorithm', fontsize=12, fontweight='bold')
    ax.set_ylabel('Execution Time (avg microseconds)', fontsize=12, fontweight='bold')
    platform = df['Platform'].iloc[0]
    ax.set_title(f"SRF Baseline Performance Characterization\nPlatform: {platform}", fontsize=14, fontweight='bold')
    
    plt.xticks(rotation=30, ha='right', fontsize=11)
    plt.grid(axis='y', linestyle='--', alpha=0.7)
    plt.tight_layout()

    os.makedirs(output_dir, exist_ok=True)
    platform_name = platform.replace(' ', '_').lower()
    output_path = os.path.join(output_dir, f"baseline_performance_{platform_name}.png")
    
    plt.savefig(output_path, dpi=300)
    print(f"Plot saved to {output_path}")

if __name__ == "__main__":
    csv_file = "results/csv/benchmark_log.csv"
    plot_dir = "results/plots"
    plot_results(csv_file, plot_dir)