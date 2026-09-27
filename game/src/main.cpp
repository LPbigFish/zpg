#include "zpg/core/Application.hpp"
#include <print>

auto main() -> int {
    std::println("Starting the game!");

    zpg::core::Application* application{new zpg::core::Application()};

    application->init();

    application->run([]() -> void {});

    return 0;
}
