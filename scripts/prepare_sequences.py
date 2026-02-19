import os
import pandas as pd

def load_seq(fasta_path):
    with open(fasta_path, 'r') as f:
        lines = f.readlines()
        return "".join([line.strip() for line in lines if not line.startswith(">")])

def prepare_sequences():
    human_raw = "datasets/sequences/raw/human_mito.fasta"
    neand_raw = "datasets/sequences/raw/neanderthal_mito.fasta"
    processed_dir = "datasets/sequences/processed"
    os.makedirs(processed_dir, exist_ok=True)

    seq_human = load_seq(human_raw)
    seq_neand = load_seq(neand_raw)

    # XS to XL ladder definitions
    scales = {
        "XS": 200,
        "S": 500,
        "M": 1000,
        "L": 2000,
        "XL": 4000
    }

    scale_data = []

    for scale, length in scales.items():
        if len(seq_human) < length or len(seq_neand) < length:
            continue
        
        # Deterministic extraction of related biological fragments
        frag_a = seq_human[:length]
        frag_b = seq_neand[:length]
        
        path_a = os.path.join(processed_dir, f"human_{scale.lower()}.txt")
        path_b = os.path.join(processed_dir, f"neand_{scale.lower()}.txt")
        
        with open(path_a, "w") as f: f.write(frag_a)
        with open(path_b, "w") as f: f.write(frag_b)
            
        scale_data.append({"scale": scale, "length": length, "path_a": path_a, "path_b": path_b})

    df = pd.DataFrame(scale_data)
    df.to_csv("datasets/metadata/sequence_scales.csv", index=False)
    print("Sequence scaling ladder (Real-world evolutionary pairs) constructed.")

if __name__ == "__main__":
    prepare_sequences()