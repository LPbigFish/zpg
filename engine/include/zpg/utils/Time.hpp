#pragma once

#include <chrono>

namespace zpg::utils {

class Time {
  public:
    Time() = delete;

    [[nodiscard]] static auto get_elapsed_time() noexcept -> float {
        static const auto start = std::chrono::steady_clock::now();
        return std::chrono::duration<float>(
                   std::chrono::steady_clock::now() - start
        )
            .count();
    }
};
} // namespace zpg::utils
