#pragma once

#include "zpg/core/Window.hpp"

namespace zpg::core {

class Application {
    Window window;

  public:
    Application() = default;
    ~Application() = default;

    Application(const Application&) = delete;
    auto operator=(const Application&) -> Application& = delete;
    Application(Application&&) = delete;
    auto operator=(Application&&) -> Application& = delete;

    /**
      Initialize window, shader program, etc.
    */
    auto init() -> void;
    auto create_shaders() -> void;
    auto create_models() -> void;
    auto run() -> void;
};

} // namespace zpg::core
