#include "regime_observer.h"

namespace srf {

void RegimeObserver::record_snapshot(long long comp, long long recomp, long long mem, size_t ws) {
    RegimeSnapshot snapshot;
    snapshot.compute_events = comp;
    snapshot.recompute_events = recomp;
    snapshot.memory_access_proxy = mem;
    snapshot.working_set_proxy = ws;
    
    snapshot.r_mem = (comp > 0) ? static_cast<double>(mem) / comp : 0.0;
    snapshot.r_rec = (comp > 0) ? static_cast<double>(recomp) / comp : 0.0;
    
    history_.push_back(snapshot);
}

RegimeSnapshot RegimeObserver::get_latest() const {
    if (history_.empty()) return {0, 0, 0, 0, 0.0, 0.0};
    return history_.back();
}

} // namespace srf
