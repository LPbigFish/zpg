#include "Application.hpp"
#include "zpg/core/GFObserver.hpp"
#include <functional>
#include <stdexcept>
#define GLAD_GL_IMPLEMENTATION
#include <GLFW/glfw3.h>
#include <glad/gl.h>
#include <memory>
#include <print>

namespace {
auto error_callback(int /*error*/, const char* description) -> void {
    std::println(stderr, "{}", description);
}
}; // namespace

namespace zpg::core {

Application::Application():
    glfw_context{GLFWContext()}, window{std::make_unique<Window>()} {}

auto Application::print_info() noexcept -> void {
    const auto* version
        // NOLINTNEXTLINE
        = reinterpret_cast<const char*>(glGetString(GL_VERSION));
    const auto* vendor
        // NOLINTNEXTLINE
        = reinterpret_cast<const char*>(glGetString(GL_VENDOR));
    const auto* renderer
        // NOLINTNEXTLINE
        = reinterpret_cast<const char*>(glGetString(GL_RENDERER));
    // NOLINTNEXTLINE
    const auto* shading_language_version = reinterpret_cast<const char*>(
        glGetString(GL_SHADING_LANGUAGE_VERSION)
    );

    std::println("OpenGL Version: {}", version);
    std::println("Vendor {}", vendor);
    std::println("Renderer {}", renderer);
    std::println("GLSL {}", shading_language_version);
    int major{};
    int minor{};
    int revision{};
    glfwGetVersion(&major, &minor, &revision);
    std::println("Using GLFW {}.{}.{}", major, minor, revision);
}

auto Application::init() -> void {
    glfwSetErrorCallback(error_callback);
    window->make_context();
    glfwSwapInterval(1);

    if (!gladLoadGL(glfwGetProcAddress)) {
        throw std::runtime_error{"GLAD initialization failed"};
    }

    int width{};
    int height{};
    glfwGetFramebufferSize(window->get(), &width, &height);
    glViewport(0, 0, width, height);

    glfwSetFramebufferSizeCallback(
        window->get(),
        // NOLINTNEXTLINE
        [](GLFWwindow*, int width, int height) -> void {
            glViewport(0, 0, width, height);
        }
    );

    Application::print_info();

    namespace gfo = zpg::core::gfobserver;
    namespace events = gfo::events;

    gfo::GlfwObserver::instance().subscribe<events::KeyEvent>(
        [](const events::KeyEvent& event) -> void {
            std::println(
                "key_callback [{},{},{},{}]",
                event.key,
                event.scancode,
                event.action,
                event.mods
            );
        }
    );
}

auto Application::run(const std::function<void()>& loop_program) -> void {
    glEnable(GL_DEPTH_TEST);
    while (!glfwWindowShouldClose(window->get())) {
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        loop_program();

        glfwSwapBuffers(window->get());
        glfwPollEvents();
    }
}
} // namespace zpg::core
