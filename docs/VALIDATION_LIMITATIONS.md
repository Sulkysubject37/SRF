# Model Validation & Limitations

## Expected Errors
- **Platform Jitter:** OS-level background processes can introduce noise in runtime measurements, causing weak correlations between predicted cost ratios and observed µs.
- **Micro-architectural Complexity:** The algebraic models do not account for L3 cache hierarchies, hardware prefetchers, or TLB misses, which can influence actual performance beyond the simple working-set model.

## Missing Data Handling
If any mandatory metric (e.g., `compute_events`) is missing from the input CSV, the driver will emit `INSUFFICIENT_DATA`. SRF forbids interpolation or fabrication of metrics.

## No Universal Accuracy Claim
The models are intended to explain trends within the SRF project. They are not intended to be general-purpose performance models for arbitrary dynamic programming implementations outside of this framework.
