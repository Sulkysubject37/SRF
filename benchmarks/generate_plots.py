import pandas as pd
import matplotlib.pyplot as plt
import os
import numpy as np

def generate_plots(csv_path, pred_path, output_dir):
    if not os.path.exists(csv_path): return
    df = pd.read_csv(csv_path)
    if df.empty: return
    
    # Merge with predictions if available
    if os.path.exists(pred_path):
        pred_df = pd.read_csv(pred_path)
        df = pd.merge(df, pred_df, on=['algorithm', 'variant', 'platform', 'backend_type', 'granularity_unit_size'], how='left')

    os.makedirs(output_dir, exist_ok=True)
    plt.style.use('seaborn-v0_8-whitegrid')

    # 1. Prediction Plot: Relative Cost Ratio vs Granularity
    if 'relative_cost_ratio' in df.columns:
        for alg in df['algorithm'].unique():
            alg_df = df[(df['algorithm'] == alg) & (df['platform'] == 'Darwin') & (df['backend_type'] == 'cpu')]
            if not alg_df.empty:
                plt.figure(figsize=(10, 6))
                valid_df = alg_df[alg_df['relative_cost_ratio'] != 'INSUFFICIENT_DATA']
                if not valid_df.empty:
                    plt.plot(valid_df['granularity_unit_size'], valid_df['relative_cost_ratio'].astype(float), marker='o', label='Model Cost Ratio')
                    plt.xlabel('Granularity Unit Size (G)')
                    plt.ylabel('Recompute/Base Cost Ratio')
                    plt.title(f'{alg}: Model-Predicted Recompute Overhead')
                    plt.legend()
                    plt.savefig(os.path.join(output_dir, f"{alg.lower().replace(' ', '_')}_model_cost.png"), dpi=300)
                plt.close()

    # 2. Locality Index Plot
    if 'locality_index' in df.columns:
        plt.figure(figsize=(12, 7))
        for alg in df['algorithm'].unique():
            alg_df = df[(df['algorithm'] == alg) & (df['platform'] == 'Darwin') & (df['backend_type'] == 'cpu')].sort_values('granularity_unit_size')
            valid_df = alg_df[alg_df['locality_index'] != 'INSUFFICIENT_DATA']
            if not valid_df.empty:
                plt.plot(valid_df['granularity_unit_size'], valid_df['locality_index'].astype(float), marker='s', label=alg)
        
        plt.xlabel('Granularity Unit Size (G)')
        plt.ylabel('Locality Index (Reuse / G)')
        plt.title('Locality Trend Analysis: Model-Predicted Index')
        plt.legend()
        plt.savefig(os.path.join(output_dir, "locality_trend_analysis.png"), dpi=300)
        plt.close()

    # 3. Observed vs Predicted: Runtime Floor (Using Darwin as reference)
    # Note: We don't predict exact runtime, but we can show the correlation between Cost Ratio and Runtime
    if 'relative_cost_ratio' in df.columns:
        plt.figure(figsize=(10, 6))
        for alg in df['algorithm'].unique():
            alg_df = df[(df['algorithm'] == alg) & (df['platform'] == 'Darwin') & (df['backend_type'] == 'cpu')]
            valid_df = alg_df[alg_df['relative_cost_ratio'] != 'INSUFFICIENT_DATA']
            if not valid_df.empty:
                plt.scatter(valid_df['relative_cost_ratio'].astype(float), valid_df['runtime_us'], label=alg)
        
        plt.xlabel('Predicted Relative Cost Ratio')
        plt.ylabel('Observed Runtime (µs)')
        plt.title('Correlation: Model Predicted Cost vs Observed Runtime')
        plt.legend()
        plt.savefig(os.path.join(output_dir, "model_runtime_correlation.png"), dpi=300)
        plt.close()

if __name__ == "__main__":
    generate_plots("results/csv/benchmark_log.csv", "results/csv/predictions.csv", "results/plots")
