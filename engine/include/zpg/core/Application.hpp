#pragma once
#include "zpg/core/GLFWContext.hpp"
#include "zpg/core/Window.hpp"
#include <functional>
#include <memory>

namespace zpg::core {

class Application {
    GLFWContext glfw_context;
    std::unique_ptr<Window> window;

    static auto print_info() noexcept -> void;

  public:
    Application();
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
    auto run(const std::function<void()>& loop_program) -> void;
};

} // namespace zpg::core
