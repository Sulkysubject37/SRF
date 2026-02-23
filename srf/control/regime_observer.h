#ifndef SRF_CONTROL_REGIME_OBSERVER_H
#define SRF_CONTROL_REGIME_OBSERVER_H

#include <vector>
#include <string>

namespace srf {

struct RegimeSnapshot {
    long long compute_events;
    long long recompute_events;
    long long memory_access_proxy;
    size_t working_set_proxy;
    double r_mem;
    double r_rec;
};

class RegimeObserver {
public:
    RegimeObserver() = default;
    
    void record_snapshot(long long comp, long long recomp, long long mem, size_t ws);
    const std::vector<RegimeSnapshot>& get_history() const { return history_; }
    RegimeSnapshot get_latest() const;

private:
    std::vector<RegimeSnapshot> history_;
};

} // namespace srf

#endif
