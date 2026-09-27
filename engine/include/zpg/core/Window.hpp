#pragma once

#include <GLFW/glfw3.h>
#include <memory>

namespace zpg::core {

class Window {
    std::unique_ptr<GLFWwindow, decltype(&glfwDestroyWindow)> window;

  public:
    Window();
    ~Window() = default;

    auto make_context() noexcept -> void;

    [[nodiscard]] auto get() noexcept -> GLFWwindow*;

    Window(const Window&) = delete;
    auto operator=(const Window&) -> Window& = delete;
    Window(Window&&) = delete;
    auto operator=(Window&&) -> Window& = delete;
};

} // namespace zpg::core
