#include "zpg/utils/poissonDisk.hpp"
#include <algorithm>
#include <cmath>
#include <cstddef>
#include <glm/ext/quaternion_geometric.hpp>
#include <glm/ext/vector_int2.hpp>
#include <numbers>
#include <random>

namespace zpg::utils {

// NOLINTNEXTLINE
auto poisson_disk(
    uint32_t seed,
    glm::vec2 min_corner,
    glm::vec2 max_corner,
    float min_distance,
    uint32_t attempts_per_point
) -> std::vector<glm::vec2> {
    if (!(min_distance > 0.f) || attempts_per_point == 0
        || !(min_corner.x < max_corner.x) || !(min_corner.y < max_corner.y)) {
        return {};
    }

    const float cell_size = min_distance / std::numbers::sqrt2_v<float>;
    const float min_distance_2 = min_distance * min_distance;
    const std::int32_t cols
        = std::ceil((max_corner.x - min_corner.x) / cell_size);
    const std::int32_t rows
        = std::ceil((max_corner.y - min_corner.y) / cell_size);

    std::vector<int32_t> grid(static_cast<std::size_t>(cols * rows), -1);
    std::vector<glm::vec2> points;
    std::vector<int32_t> active;

    std::mt19937 rng{seed};
    std::uniform_real_distribution<float> unit(0.f, 1.f);

    auto cell_of = [&](glm::vec2 point) -> glm::ivec2 {
        return {
          std::clamp(
              static_cast<int32_t>((point.x - min_corner.x) / cell_size),
              0,
              cols - 1
          ),
          std::clamp(
              static_cast<int32_t>((point.y - min_corner.y) / cell_size),
              0,
              rows - 1
          )
        };
    };

    auto grid_at = [&](int32_t x, int32_t y) -> int32_t& {
        return grid.at((static_cast<size_t>(y) * cols) + x);
    };

    auto add_point = [&](glm::vec2 point) -> void {
        const auto index = static_cast<int32_t>(points.size());
        points.push_back(point);
        active.push_back(index);

        const glm::ivec2 cell = cell_of(point);
        grid_at(cell.x, cell.y) = index;
    };

    auto valid = [&](glm::vec2 point) -> bool {
        if (point.x < min_corner.x || point.x >= max_corner.x
            || point.y < min_corner.y || point.y >= max_corner.y) {
            return false;
        }

        const glm::ivec2 cell = cell_of(point);
        for (int32_t y = std::max(0, cell.y - 2);
             y <= std::min(cols - 1, cell.y + 2);
             y++) {
            for (int32_t x = std::max(0, cell.x - 2);
                 x <= std::min(cols - 1, cell.x + 2);
                 x++) {
                const int32_t index = grid_at(x, y);
                if (index == -1) {
                    continue;
                }

                const glm::vec2 distance = point - points.at(index);
                if (glm::dot(distance, distance) < min_distance_2) {
                    return false;
                }
            }
        }
        return true;
    };

    add_point({
      min_corner.x + (unit(rng) * (max_corner.x - min_corner.x)),
      min_corner.y + (unit(rng) * (max_corner.y - min_corner.y)),
    });

    while (!active.empty()) {
        std::uniform_int_distribution<std::size_t> pick(0, active.size() - 1);
        const std::size_t slot = pick(rng);
        const glm::vec2 center = points.at(active.at(slot));
        bool generated = false;

        for (uint32_t attempt = 0; attempt < attempts_per_point; attempt++) {
            const float theta = 2.f * std::numbers::pi_v<float> * unit(rng);
            const float radius
                = min_distance * std::sqrt(1.f + (3.f * unit(rng)));
            const glm::vec2 candidate
                = center + radius * glm::vec2{std::cos(theta), std::sin(theta)};

            if (valid(candidate)) {
                add_point(candidate);
                generated = true;
                break;
            }
        }

        if (!generated) {
            active[slot] = active.back();
            active.pop_back();
        }
    }

    return points;
}

} // namespace zpg::utils
