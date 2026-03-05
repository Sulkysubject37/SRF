# Submission Cover Letter

**Date:** March 5, 2026  
**To:** The Editor-in-Chief, *Algorithms for Molecular Biology*

**Subject:** Submission of Manuscript for Publication

Dear Editor,

I am pleased to submit the manuscript entitled **"Structural Recomputation Framework: A Deterministic Execution Abstraction for Memory-Constrained Dynamic Programming in Bioinformatics"** for consideration as a research article in *Algorithms for Molecular Biology*.

This work introduces the Structured Recomputation Framework (SRF), a formal execution-level abstraction designed to resolve the quadratic memory constraints inherent in many dynamic programming (DP) algorithms. Unlike algorithm-specific modifications, SRF provides a recurrence-agnostic engine that transforms the execution schedule of a DP Directed Acyclic Graph (DAG) into a memory-bounded frontier. The framework is instantiated and evaluated across three distinct paradigms: global sequence alignment (Needleman-Wunsch), Hidden Markov Model inference (Forward and Viterbi), and graph-based dynamic programming. 

The contribution of this work lies in the formalization of recomputation-based memory reduction as a managed system property. We demonstrate that peak working set sizes can be reduced from $O(N^2)$ or $O(V)$ to $O(N)$ or $O(	ext{width}(\mathcal{G}))$, respectively, enabling exact genomic analyses on commodity hardware. Furthermore, the framework enforces strict deterministic execution, verified through cross-platform and cross-compiler invariance checks to ensure bit-exact reproducibility.

This manuscript aligns with the scope of *Algorithms for Molecular Biology* by presenting significant algorithmic engineering and high-performance computing strategies for fundamental molecular biology problems. I confirm that this manuscript is original, has not been published previously, and is not currently under consideration for publication elsewhere. All code, benchmark pipelines, and reproducibility resources are available in a public repository to facilitate independent validation.

Thank you for your time and consideration of this work.

Sincerely,

**MD. Arshad**  
Department of Computer Science  
Jamia Millia Islamia  
New Delhi, India  
