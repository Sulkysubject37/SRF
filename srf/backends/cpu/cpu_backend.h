#ifndef SRF_BACKENDS_CPU_BACKEND_H
#define SRF_BACKENDS_CPU_BACKEND_H

#include "../backend_interface.h"
#include <algorithm>
#include <cstring>

namespace srf {

class CpuBackend : public IBackend {
public:
    BackendType type() const override { return BackendType::CPU; }

    void* allocate_device(size_t size) override { return malloc(size); }
    void free_device(void* ptr) override { free(ptr); }
    void copy_to_device(void* dst, const void* src, size_t size) override { memcpy(dst, src, size); }
    void copy_to_host(void* dst, const void* src, size_t size) override { memcpy(dst, src, size); }

    int nw_cell_compute(int diag, int top, int left, int match_score, int gap_penalty) override {
        return std::max({diag + match_score, top + gap_penalty, left + gap_penalty});
    }

    double hmm_step_compute(const std::vector<double>& prev_alpha, const std::vector<double>& trans_row, double emission) override {
        double sum = 0.0;
        for (size_t i = 0; i < prev_alpha.size(); ++i) {
            sum += prev_alpha[i] * trans_row[i] * emission;
        }
        return sum;
    }

    int graph_node_compute(const std::vector<int>& predecessor_dists, const std::vector<int>& weights) override {
        int min_dist = 1e9;
        for (size_t i = 0; i < predecessor_dists.size(); ++i) {
            min_dist = std::min(min_dist, predecessor_dists[i] + weights[i]);
        }
        return min_dist;
    }

    BackendMetrics get_metrics() const override { return {0, 0, 0}; }
    void reset_metrics() override {}
};

} // namespace srf

#endif
