#ifndef SRF_CONTROL_ADAPTATION_POLICY_H
#define SRF_CONTROL_ADAPTATION_POLICY_H

#include "drift_detector.h"

namespace srf {

struct AdaptationSignal {
    bool should_adapt = false;
    int delta = 0;
    std::string reason = "NONE";
};

class AdaptationPolicy {
public:
    AdaptationPolicy(int cooldown_steps = 100) 
        : cooldown_steps_(cooldown_steps), current_cooldown_(0) {}

    AdaptationSignal evaluate(DriftState drift, const RegimeObserver& observer);

private:
    int cooldown_steps_;
    int current_cooldown_;
    double last_r_rec_ = -1.0;
};

} // namespace srf

#endif
