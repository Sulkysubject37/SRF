#include "backend_selector.h"
#include "../backends/cpu/cpu_backend.h"
#include "../backends/gpu/gpu_backend.h"
#include <cstdlib>

namespace srf {

std::unique_ptr<IBackend> BackendSelector::select(size_t device_budget_kb) {
    // Simulation of GPU backend selection.
    const char* force_gpu = std::getenv("SRF_FORCE_GPU");
    const char* force_cpu = std::getenv("SRF_FORCE_CPU");

    if (force_cpu) {
        return std::make_unique<CpuBackend>();
    }

    // A GPU backend can be simulated for testing by setting this environment variable.
    if (force_gpu && std::string(force_gpu) == "1") {
        return std::make_unique<GpuBackend>(device_budget_kb);
    }

    // Default to CPU backend.
    return std::make_unique<CpuBackend>();
}

} // namespace srf
