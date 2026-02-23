#ifndef SRF_CONTROL_DRIFT_DETECTOR_H
#define SRF_CONTROL_DRIFT_DETECTOR_H

#include "regime_observer.h"

namespace srf {

enum class DriftState {
    STABLE,
    DRIFT_CANDIDATE,
    INSUFFICIENT_DATA
};

class DriftDetector {
public:
    DriftDetector(size_t window_size = 5) : window_size_(window_size) {}
    
    DriftState detect(const RegimeObserver& observer);
    
private:
    size_t window_size_;
};

} // namespace srf

#endif
