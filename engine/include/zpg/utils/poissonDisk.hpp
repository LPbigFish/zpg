#pragma once

#include <cstdint>
#include <glm/ext/vector_float2.hpp>
#include <vector>

namespace zpg::utils {

// NOLINTNEXTLINE
auto poisson_disk(
    uint32_t seed,
    glm::vec2 min_corner,
    glm::vec2 max_corner,
    float min_distance,
    uint32_t attempts_per_point = 30
) -> std::vector<glm::vec2>;

} // namespace zpg::utils
