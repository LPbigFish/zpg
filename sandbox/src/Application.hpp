#pragma once

#include "zpg/core/GLFWContext.hpp"
#include "zpg/core/Scene.hpp"
#include "zpg/core/Window.hpp"
#include <cstddef>
#include <functional>
#include <memory>
#include <vector>

namespace zpg::core {

class Application {
    GLFWContext glfw_context;
    std::unique_ptr<Window> window;

    std::vector<std::unique_ptr<zpg::core::Scene>> scenes;
    std::size_t current_scene_index{0};

    static auto print_info() noexcept -> void;

    auto add_scene(std::unique_ptr<zpg::core::Scene> scene) -> void;
    auto switch_scene(std::size_t index) -> void;
    auto create_callbacks() -> void;

  public:
    Application();
    ~Application() = default;

    Application(const Application&) = delete;
    auto operator=(const Application&) -> Application& = delete;
    Application(Application&&) = delete;
    auto operator=(Application&&) -> Application& = delete;

    auto init() -> void;

    auto create_scenes() -> void;

    auto run() -> void;
    auto run(const std::function<void()>& loop_program) -> void;
};

} // namespace zpg::core
