#ifndef SRF_GRANULARITY_GRANULARITY_POLICY_H
#define SRF_GRANULARITY_GRANULARITY_POLICY_H

#include <string>

namespace srf {

enum class GranularityType {
    TILE,       // For NW (2D)
    SEGMENT,    // For HMM (1D)
    GROUP       // For Graph (Set-based)
};

class GranularityPolicy {
public:
    GranularityPolicy(GranularityType type, int unit_size) 
        : type_(type), unit_size_(unit_size) {}

    GranularityType type() const { return type_; }
    int unit_size() const { return unit_size_; }

    // Returns a unique identifier for the granularity unit containing 'idx'
    long long get_unit_id(long long idx) const {
        return idx / unit_size_;
    }

    // Returns a unique identifier for a 2D unit (for NW tiles)
    long long get_unit_id_2d(long long row, long long col) const {
        return (row / unit_size_) * 1000000LL + (col / unit_size_);
    }

    // Returns the boundary of the current unit
    long long get_unit_boundary(long long idx) const {
        return ((idx / unit_size_) + 1) * unit_size_;
    }

    std::string to_string() const {
        switch (type_) {
            case GranularityType::TILE: return "tile";
            case GranularityType::SEGMENT: return "segment";
            case GranularityType::GROUP: return "group";
            default: return "unknown";
        }
    }

private:
    GranularityType type_;
    int unit_size_;
};

} // namespace srf

#endif
