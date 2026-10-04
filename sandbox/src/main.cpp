#include "Application.hpp"

namespace zc = zpg::core;

auto main() -> int {
    zc::Application app{};

    app.init();

    app.create_scenes();

    app.run();

    return 0;
}
