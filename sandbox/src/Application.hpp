#pragma once

#include "zpg/core/GLFWContext.hpp"
#include "zpg/core/InputManager.hpp"
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

    InputManager input_manager;

    // Task 2c/2d: toggles for the resize demo (F3 = viewport, F4 = aspect).
    bool update_viewport_on_resize{true};
    bool update_projection_on_resize{true};

    static auto print_info() noexcept -> void;

    auto refresh_window_title() const -> void;

    auto add_scene(std::unique_ptr<zpg::core::Scene> scene) -> void;
    auto switch_scene(std::size_t index) -> void;
    auto create_callbacks() -> void;

  public:
    Application();
    ~Application();

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
