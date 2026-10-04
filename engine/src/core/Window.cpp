#include "zpg/core/Window.hpp"
#include "zpg/core/GFObserver.hpp"
#include <GLFW/glfw3.h>
#include <stdexcept>

namespace zpg::core {
Window::Window():
    window{
      glfwCreateWindow(1280, 960, "ZPG Window", nullptr, nullptr),
      &glfwDestroyWindow
    } {
    if (!window) {
        throw std::runtime_error{"Failed to initialize window"};
    }

    glfwSetKeyCallback(
        window.get(),
        // NOLINTNEXTLINE
        [](GLFWwindow* /*window*/, int key, int scancode, int action, int mods)
            -> void {
            auto& observer = gfobserver::GlfwObserver::instance();

            observer.notify(
                gfobserver::events::KeyEvent{
                  .key = key,
                  .scancode = scancode,
                  .action = action,
                  .mods = mods
                }
            );
        }
    );
}

auto Window::make_context() noexcept -> void {
    glfwMakeContextCurrent(window.get());
}

auto Window::get() noexcept -> GLFWwindow* {
    return this->window.get();
}
} // namespace zpg::core
