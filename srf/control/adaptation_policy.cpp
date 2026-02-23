#include "adaptation_policy.h"

namespace srf {

AdaptationSignal AdaptationPolicy::evaluate(DriftState drift, const RegimeObserver& observer) {
    AdaptationSignal signal;
    
    if (current_cooldown_ > 0) {
        current_cooldown_--;
        return signal;
    }

    if (drift != DriftState::DRIFT_CANDIDATE) {
        return signal;
    }

    const auto& history = observer.get_history();
    if (history.size() < 2) return signal;

    RegimeSnapshot latest = history.back();
    
    // Hysteresis: Only adapt if current ratio is significantly different from last adaptation point
    if (last_r_rec_ > 0 && std::abs(latest.r_rec - last_r_rec_) < (last_r_rec_ * 0.05)) {
        return signal;
    }

    // Conservative logic: If recompute ratio is high, slightly increase granularity/checkpoint interval
    // to amortize management overhead further.
    if (latest.r_rec > 0.5) {
        signal.should_adapt = true;
        signal.delta = 1; // Increase param by 1
        signal.reason = "HIGH_RECOMPUTE_RATIO";
    } else if (latest.r_rec < 0.1) {
        signal.should_adapt = true;
        signal.delta = -1; // Decrease param by 1
        signal.reason = "LOW_RECOMPUTE_UTILIZATION";
    }

    if (signal.should_adapt) {
        last_r_rec_ = latest.r_rec;
        current_cooldown_ = cooldown_steps_;
    }

    return signal;
}

} // namespace srf
