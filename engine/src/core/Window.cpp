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

    using GO = gfobserver::GlfwObserver;
    namespace events = gfobserver::events;

    glfwSetKeyCallback(
        window.get(),
        // NOLINTNEXTLINE
        [](GLFWwindow*, int key, int scancode, int action, int mods) -> void {
            GO::instance().notify(
                events::KeyEvent{
                  .key = key,
                  .scancode = scancode,
                  .action = action,
                  .mods = mods
                }
            );
        }
    );

    glfwSetMouseButtonCallback(
        window.get(),
        // NOLINTNEXTLINE
        [](GLFWwindow*, int button, int action, int mods) -> void {
            GO::instance().notify(
                events::ButtonEvent{
                  .button = button, .action = action, .mods = mods
                }
            );
        }
    );

    glfwSetCursorPosCallback(
        window.get(),
        // NOLINTNEXTLINE
        [](GLFWwindow*, double x, double y) {
            GO::instance().notify(events::CursorEvent{.x = x, .y = y});
        }
    );

    glfwSetWindowSizeCallback(
        window.get(),
        // NOLINTNEXTLINE
        [](GLFWwindow*, int x, int y) -> void {
            GO::instance().notify(
                events::WindowSizeEvent{.width = x, .height = y}
            );
        }
    );

    glfwSetFramebufferSizeCallback(
        window.get(),
        // NOLINTNEXTLINE
        [](GLFWwindow*, int x, int y) -> void {
            GO::instance().notify(
                events::FrameBufferSizeEvent{.width = x, .height = y}
            );
        }
    );

    glfwSetWindowFocusCallback(
        window.get(), [](GLFWwindow*, int focused) -> void {
            GO::instance().notify(events::WindowFocusEvent{focused});
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
