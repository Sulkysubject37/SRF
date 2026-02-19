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
        # We need to merge on columns that exist in both
        df = pd.merge(df, pred_df, on=['algorithm', 'variant', 'platform', 'backend_type', 'granularity_unit_size'], how='left')

    os.makedirs(output_dir, exist_ok=True)
    plt.style.use('seaborn-v0_8-whitegrid')

    # 1. Scaling Plot: Runtime vs Dataset Scale
    plt.figure(figsize=(12, 7))
    for alg in df['algorithm'].unique():
        alg_df = df[df['algorithm'] == alg].sort_values('input_size')
        if not alg_df.empty:
            plt.plot(alg_df['input_size'], alg_df['runtime_us'], marker='o', linewidth=2, label=alg)
    
    plt.xlabel('Input Size (Length / Nodes)', fontsize=12, fontweight='bold')
    plt.ylabel('Runtime (µs)', fontsize=12, fontweight='bold')
    plt.title('Phase 6: Runtime Scaling across Biological Workloads (XS to XL)', fontsize=14, fontweight='bold')
    plt.legend()
    plt.yscale('log')
    plt.savefig(os.path.join(output_dir, "workload_runtime_scaling.png"), dpi=300)
    plt.close()

    # 2. Recomputation Scaling
    plt.figure(figsize=(12, 7))
    for alg in df['algorithm'].unique():
        alg_df = df[df['algorithm'] == alg].sort_values('input_size')
        if not alg_df.empty:
            plt.plot(alg_df['input_size'], alg_df['recompute_events'], marker='s', linewidth=2, label=alg)
    
    plt.xlabel('Input Size', fontsize=12, fontweight='bold')
    plt.ylabel('Recomputation Events', fontsize=12, fontweight='bold')
    plt.title('Phase 6: Recomputation Events vs Input Scale', fontsize=14, fontweight='bold')
    plt.legend()
    plt.yscale('log')
    plt.savefig(os.path.join(output_dir, "workload_recompute_scaling.png"), dpi=300)
    plt.close()

    # 3. Model Validation: Predicted vs Observed Cost
    if 'relative_cost_ratio' in df.columns:
        plt.figure(figsize=(10, 6))
        for alg in df['algorithm'].unique():
            alg_df = df[df['algorithm'] == alg]
            valid_df = alg_df[alg_df['relative_cost_ratio'] != 'INSUFFICIENT_DATA']
            if not valid_df.empty:
                plt.scatter(valid_df['relative_cost_ratio'].astype(float), valid_df['runtime_us'], label=alg, s=100, alpha=0.7)
        
        plt.xlabel('Predicted Cost Ratio', fontsize=12, fontweight='bold')
        plt.ylabel('Observed Runtime (µs)', fontsize=12, fontweight='bold')
        plt.title('Phase 6: Model Cost Ratio vs Observed Workload Runtime', fontsize=14, fontweight='bold')
        plt.legend()
        plt.yscale('log')
        plt.savefig(os.path.join(output_dir, "workload_model_validation.png"), dpi=300)
        plt.close()

if __name__ == "__main__":
    generate_plots("results/csv/benchmark_log.csv", "results/csv/predictions.csv", "results/plots")