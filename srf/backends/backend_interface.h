#ifndef SRF_BACKENDS_BACKEND_INTERFACE_H
#define SRF_BACKENDS_BACKEND_INTERFACE_H

#include <vector>
#include <string>
#include <memory>

namespace srf {

enum class BackendType {
    CPU,
    GPU
};

struct BackendMetrics {
    long long transfer_overhead_us = 0;
    long long kernel_launch_count = 0;
    size_t device_memory_budget_kb = 0;
};

class IBackend {
public:
    virtual ~IBackend() = default;
    virtual BackendType type() const = 0;
    
    // Memory Management
    virtual void* allocate_device(size_t size) = 0;
    virtual void free_device(void* ptr) = 0;
    virtual void copy_to_device(void* dst, const void* src, size_t size) = 0;
    virtual void copy_to_host(void* dst, const void* src, size_t size) = 0;

    // Primitives
    virtual int nw_cell_compute(int diag, int top, int left, int match_score, int gap_penalty) = 0;
    virtual double hmm_step_compute(const std::vector<double>& prev_alpha, const std::vector<double>& trans_row, double emission) = 0;
    virtual int graph_node_compute(const std::vector<int>& predecessor_dists, const std::vector<int>& weights) = 0;

    virtual BackendMetrics get_metrics() const = 0;
    virtual void reset_metrics() = 0;
};

} // namespace srf

#endif
