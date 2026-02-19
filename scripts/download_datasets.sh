#!/usr/bin/env bash
set -e

echo "[SRF] Downloading REAL Biological Dataset Pairs..."

# 1. Sequences for HMM and Needleman-Wunsch
# We download a pair of mitochondrial genomes for comparative alignment
mkdir -p datasets/sequences/raw

# Sequence 1: Human Mitochondrial Genome (NC_012920.1)
if [ ! -f "datasets/sequences/raw/human_mito.fasta" ]; then
    curl -s "https://eutils.ncbi.nlm.nih.gov/entrez/eutils/efetch.fcgi?db=nuccore&id=NC_012920.1&rettype=fasta" > "datasets/sequences/raw/human_mito.fasta"
    echo "Downloaded Human Mitochondrial Sequence."
fi

# Sequence 2: Neanderthal Mitochondrial Genome (NC_011137.1)
if [ ! -f "datasets/sequences/raw/neanderthal_mito.fasta" ]; then
    curl -s "https://eutils.ncbi.nlm.nih.gov/entrez/eutils/efetch.fcgi?db=nuccore&id=NC_011137.1&rettype=fasta" > "datasets/sequences/raw/neanderthal_mito.fasta"
    echo "Downloaded Neanderthal Mitochondrial Sequence."
fi

# 2. DAG for Graph-DP
# Source: Gene Ontology (GO) - Basic OBO
mkdir -p datasets/graphs/raw
if [ ! -f "datasets/graphs/raw/go-basic.obo" ]; then
    curl -s "http://purl.obolibrary.org/obo/go/go-basic.obo" > "datasets/graphs/raw/go-basic.obo"
    echo "Downloaded Gene Ontology OBO."
fi

echo "[SRF] Real-world dataset acquisition complete."
