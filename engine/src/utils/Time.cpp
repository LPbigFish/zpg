#include "zpg/utils/Time.hpp"
#include <chrono>

namespace zpg::utils {

auto Time::get_elapsed_time() noexcept -> float {
    static const auto start = std::chrono::steady_clock::now();
    return std::chrono::duration<float>(
               std::chrono::steady_clock::now() - start
    )
        .count();
}

} // namespace zpg::utils
