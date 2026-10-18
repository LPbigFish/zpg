#pragma once

namespace zpg::utils {

class Time {
  public:
    Time() = delete;

    [[nodiscard]] static auto get_elapsed_time() noexcept -> float;
};
} // namespace zpg::utils
