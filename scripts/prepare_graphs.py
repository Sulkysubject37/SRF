import os
import pandas as pd
import re

def parse_go_dag(obo_path):
    # Simplistic parser for GO OBO to extract is_a relationships (DAG)
    # is_a: GO:XXXXXXX ! term name
    edges = []
    current_term = None
    
    # Map GO IDs to integers for Graph-DP internal format
    go_to_int = {}
    next_id = 0

    with open(obo_path, 'r') as f:
        for line in f:
            line = line.strip()
            if line.startswith("id: GO:"):
                current_term = line.split("id: ")[1]
                if current_term not in go_to_int:
                    go_to_int[current_term] = next_id
                    next_id += 1
            elif line.startswith("is_a: GO:") and current_term:
                parent = line.split("is_a: ")[1].split(" !")[0]
                if parent not in go_to_int:
                    go_to_int[parent] = next_id
                    next_id += 1
                # Edge: Parent -> Child (Biological direction) or Child -> Parent (Traversal)
                # For DP, we typically traverse dependency: Child depends on Parent.
                edges.append((go_to_int[parent], go_to_int[current_term], 1))
    
    return edges, next_id

def prepare_graphs():
    obo_path = "datasets/graphs/raw/go-basic.obo"
    processed_dir = "datasets/graphs/processed"
    os.makedirs(processed_dir, exist_ok=True)

    if not os.path.exists(obo_path):
        print("GO OBO not found.")
        return

    all_edges, total_nodes = parse_go_dag(obo_path)

    # XS to XL ladder definitions
    scales = {
        "XS": 500,
        "S": 1000,
        "M": 2000,
        "L": 5000,
        "XL": 10000
    }

    scale_data = []

    for scale, nodes_limit in scales.items():
        # Deterministically select first N nodes and their internal edges
        # This preserves the top-level hierarchical structure
        subset_edges = [e for e in all_edges if e[0] < nodes_limit and e[1] < nodes_limit]
        
        graph_path = os.path.join(processed_dir, f"go_subset_{scale.lower()}.txt")
        with open(graph_path, "w") as f:
            for u, v, w in subset_edges:
                f.write(f"{u} {v} {w}\n")
        
        scale_data.append({"scale": scale, "nodes": nodes_limit, "edge_count": len(subset_edges), "path": graph_path})

    df = pd.DataFrame(scale_data)
    df.to_csv("datasets/metadata/graph_scales.csv", index=False)
    print("Graph scaling ladder (Gene Ontology Hierarchy) constructed.")

if __name__ == "__main__":
    prepare_graphs()