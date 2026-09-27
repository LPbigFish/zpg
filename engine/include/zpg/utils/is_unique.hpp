#pragma once
#include <concepts>
#include <type_traits>

namespace zpg::utils {
template<typename T, typename... Ts>
concept different_from_all = (!std::same_as<T, Ts> && ...);

template<typename... Ts> struct IsUniqueV: std::true_type {};

template<typename T, typename... Ts>
struct IsUniqueV<T, Ts...>
    : std::bool_constant<
          different_from_all<T, Ts...> && IsUniqueV<Ts...>::value> {};

template<typename... Ts>
concept is_unique = IsUniqueV<Ts...>::value;

static_assert(is_unique<float, int>);
static_assert(!is_unique<float, float>);
} // namespace zpg::utils
