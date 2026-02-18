#ifndef SRF_BACKENDS_GPU_BACKEND_H
#define SRF_BACKENDS_GPU_BACKEND_H

#include "../backend_interface.h"
#include <algorithm>
#include <cstring>

namespace srf {

class GpuBackend : public IBackend {
private:
    BackendMetrics metrics;
public:
    GpuBackend(size_t budget_kb) {
        metrics.device_memory_budget_kb = budget_kb;
    }

    BackendType type() const override { return BackendType::GPU; }

    void* allocate_device(size_t size) override { return malloc(size); }
    void free_device(void* ptr) override { free(ptr); }
    
    void copy_to_device(void* dst, const void* src, size_t size) override { 
        metrics.transfer_overhead_us += 10;
        memcpy(dst, src, size); 
    }
    void copy_to_host(void* dst, const void* src, size_t size) override { 
        metrics.transfer_overhead_us += 10;
        memcpy(dst, src, size); 
    }

    int nw_cell_compute(int diag, int top, int left, int match_score, int gap_penalty) override {
        metrics.kernel_launch_count++;
        return std::max({diag + match_score, top + gap_penalty, left + gap_penalty});
    }

    double forward_step_compute(const std::vector<double>& prev_alpha, const std::vector<double>& trans_row, double emission) override {
        metrics.kernel_launch_count++;
        double sum = 0.0;
        for (size_t i = 0; i < prev_alpha.size(); ++i) {
            sum += prev_alpha[i] * trans_row[i] * emission;
        }
        return sum;
    }

    double viterbi_step_compute(const std::vector<double>& prev_v, const std::vector<double>& trans_row, double emission) override {
        metrics.kernel_launch_count++;
        double max_val = -1.0;
        for (size_t i = 0; i < prev_v.size(); ++i) {
            max_val = std::max(max_val, prev_v[i] * trans_row[i] * emission);
        }
        return max_val;
    }

    int graph_node_compute(const std::vector<int>& predecessor_dists, const std::vector<int>& weights) override {
        metrics.kernel_launch_count++;
        int min_dist = 1e9;
        for (size_t i = 0; i < predecessor_dists.size(); ++i) {
            min_dist = std::min(min_dist, predecessor_dists[i] + weights[i]);
        }
        return min_dist;
    }

    BackendMetrics get_metrics() const override { return metrics; }
    void reset_metrics() override {
        size_t budget = metrics.device_memory_budget_kb;
        metrics = {0, 0, budget};
    }
};

} // namespace srf

#endif