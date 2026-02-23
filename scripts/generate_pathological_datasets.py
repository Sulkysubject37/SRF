import os
import random

def generate_seq(length, seed, path):
    random.seed(seed)
    bases = ['A', 'C', 'G', 'T']
    with open(path, 'w') as f:
        f.write("".join(random.choice(bases) for _ in range(length)))

def generate_graph(nodes, mode, path):
    # Mode: 'path' (linear) or 'dense' (high fan-in)
    with open(path, 'w') as f:
        if mode == 'path':
            for u in range(nodes - 1):
                f.write(f"{u} {u+1} 1\n")
        elif mode == 'dense':
            for u in range(nodes):
                for v in range(u + 1, min(u + 21, nodes)):
                    f.write(f"{u} {v} 1\n")

def main():
    path_dir = "datasets_extreme/pathological"
    os.makedirs(path_dir, exist_ok=True)

    # 1. Tiny Workloads
    generate_seq(10, 10, os.path.join(path_dir, "seq_tiny_a.txt"))
    generate_seq(10, 11, os.path.join(path_dir, "seq_tiny_b.txt"))
    generate_graph(10, 'path', os.path.join(path_dir, "graph_tiny.txt"))

    # 2. Skewed Dimensions
    generate_seq(1000, 1000, os.path.join(path_dir, "seq_skewed_long.txt"))
    generate_seq(10, 10, os.path.join(path_dir, "seq_skewed_short.txt"))

    # 3. Graph Topology Extremes
    generate_graph(1000, 'path', os.path.join(path_dir, "graph_path.txt"))
    generate_graph(1000, 'dense', os.path.join(path_dir, "graph_dense.txt"))

    print("Pathological datasets generated.")

if __name__ == "__main__":
    main()
