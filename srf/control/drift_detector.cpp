#include "drift_detector.h"
#include <numeric>
#include <cmath>

namespace srf {

DriftState DriftDetector::detect(const RegimeObserver& observer) {
    const auto& history = observer.get_history();
    if (history.size() < window_size_) return DriftState::INSUFFICIENT_DATA;

    // Evaluate drift in R_rec (Recompute ratio)
    std::vector<double> r_recs;
    for (size_t i = history.size() - window_size_; i < history.size(); ++i) {
        r_recs.push_back(history[i].r_rec);
    }

    double sum = std::accumulate(r_recs.begin(), r_recs.end(), 0.0);
    double mean = sum / window_size_;

    double sq_sum = std::inner_product(r_recs.begin(), r_recs.end(), r_recs.begin(), 0.0);
    double stdev = std::sqrt(sq_sum / window_size_ - mean * mean);

    // Check for sustained monotonic drift (relative deviation)
    bool increasing = true;
    bool decreasing = true;
    for (size_t i = 1; i < r_recs.size(); ++i) {
        if (r_recs[i] < r_recs[i-1]) increasing = false;
        if (r_recs[i] > r_recs[i-1]) decreasing = false;
    }

    if ((increasing || decreasing) && stdev > (mean * 0.01)) {
        return DriftState::DRIFT_CANDIDATE;
    }

    return DriftState::STABLE;
}

} // namespace srf
