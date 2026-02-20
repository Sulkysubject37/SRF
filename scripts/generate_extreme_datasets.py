import os
import random
import pandas as pd

def generate_sequence(length, seed, path):
    random.seed(seed)
    bases = ['A', 'C', 'G', 'T']
    with open(path, 'w') as f:
        seq = "".join(random.choice(bases) for _ in range(length))
        f.write(seq)

def generate_dag(nodes, seed, path):
    random.seed(seed)
    with open(path, 'w') as f:
        # Mostly sequential with jump edges
        for u in range(nodes - 1):
            f.write(f"{u} {u+1} 1\n")
            if random.random() < 0.1 and u + 10 < nodes:
                f.write(f"{u} {u + random.randint(2, 10)} 1\n")

def main():
    base_dir = "datasets_extreme"
    seq_dir = os.path.join(base_dir, "sequences")
    graph_dir = os.path.join(base_dir, "graphs")
    
    seq_scales = [10000, 50000, 100000, 500000, 1000000]
    graph_scales = [10000, 50000, 100000, 250000]
    
    seq_metadata = []
    for s in seq_scales:
        path_a = os.path.join(seq_dir, f"seq_a_{s}.txt")
        path_b = os.path.join(seq_dir, f"seq_b_{s}.txt")
        generate_sequence(s, s, path_a)
        generate_sequence(s, s + 1, path_b)
        seq_metadata.append({"length": s, "path_a": path_a, "path_b": path_b})
        
    graph_metadata = []
    for s in graph_scales:
        path = os.path.join(graph_dir, f"graph_{s}.txt")
        generate_dag(s, s, path)
        graph_metadata.append({"nodes": s, "path": path})
        
    pd.DataFrame(seq_metadata).to_csv("datasets_extreme/metadata/sequence_scales.csv", index=False)
    pd.DataFrame(graph_metadata).to_csv("datasets_extreme/metadata/graph_scales.csv", index=False)
    print("Extreme scale datasets generated.")

if __name__ == "__main__":
    main()