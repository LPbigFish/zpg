#include "Application.hpp"
#include <print>

auto main() -> int {
    std::println("Starting the game!");

    zpg::core::Application application{};

    application.init();

    application.run([]() -> void {});

    return 0;
}
