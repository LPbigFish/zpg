#include "zpg/core/Window.hpp"
#include <GLFW/glfw3.h>
#include <stdexcept>

// TODO: Create GLFW window and request GLFW_OPENGL_DEBUG_CONTEXT in debug
// builds.

namespace zpg::core {
Window::Window():
    window{
      glfwCreateWindow(1280, 960, "ZPG Window", nullptr, nullptr),
      &glfwDestroyWindow
    } {
    if (!window) {
        throw std::runtime_error{"Failed to initialize window"};
    }
}

auto Window::make_context() noexcept -> void {
    glfwMakeContextCurrent(window.get());
}

auto Window::get() noexcept -> GLFWwindow* {
    return this->window.get();
}
} // namespace zpg::core
