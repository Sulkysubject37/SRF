#ifndef SRF_RUNTIME_BACKEND_SELECTOR_H
#define SRF_RUNTIME_BACKEND_SELECTOR_H

#include "../backends/backend_interface.h"
#include <memory>

namespace srf {

class BackendSelector {
public:
    static std::unique_ptr<IBackend> select(size_t device_budget_kb = 1024);
};

} // namespace srf

#endif
