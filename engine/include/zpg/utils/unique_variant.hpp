#pragma once
#include "is_unique.hpp"
#include <variant>

namespace zpg::utils {

template<typename... Ts>
    requires is_unique<Ts...>
using uniqueVariant = std::variant<Ts...>;
}
