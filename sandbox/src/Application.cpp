#include "Application.hpp"
#include "SceneBuilder.hpp"
#include "zpg/core/GFObserver.hpp"
#include "zpg/core/Renderer.hpp"
#include "zpg/core/Scene.hpp"
#include "zpg/graphics/Shader.hpp"
#include "zpg/graphics/ShaderProgram.hpp"
#include <algorithm>
#include <cstddef>
#include <cstdlib>
#include <glm/ext/quaternion_transform.hpp>
#include <glm/ext/vector_float3.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/trigonometric.hpp>
#include <stdexcept>
#include <utility>
#define GLAD_GL_IMPLEMENTATION
#include <GLFW/glfw3.h>
#include <glad/gl.h>
#include <memory>
#include <print>

namespace zpg::core {

// NOLINTNEXTLINE
namespace zg = zpg::graphics;

namespace zc = zpg::core;

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
    Application::create_callbacks();
    window->make_context();

    glfwSwapInterval(1);

    if (!gladLoadGL(glfwGetProcAddress)) {
        throw std::runtime_error{"GLAD initialization failed"};
    }

    int width{};
    int height{};
    glfwGetFramebufferSize(window->get(), &width, &height);

    Renderer::set_viewport(width, height);

    glfwSetFramebufferSizeCallback(
        window->get(),
        // NOLINTNEXTLINE
        [](GLFWwindow*, int width, int height) -> void {
            Renderer::set_viewport(width, height);
        }
    );

    Application::print_info();
}

auto Application::create_callbacks() -> void {
    namespace gfo = zpg::core::gfobserver;
    namespace events = gfo::events;

    gfo::GlfwObserver::instance().subscribe<events::Error>(
        [](const events::Error& event) -> void {
            std::println(
                stderr,
                "error_callback [{}]: {}",
                event.error,
                event.description
            );
        }
    );

    gfo::GlfwObserver::instance().subscribe<events::KeyEvent>(
        [this](const events::KeyEvent& event) -> void {
            if (event.key > GLFW_KEY_0 && event.key <= GLFW_KEY_9
                && event.action == GLFW_PRESS) {
                auto index = static_cast<std::size_t>(event.key - GLFW_KEY_1);
                this->switch_scene(index);
                std::println("Switched to scene {}", index);
            }
        }
    );
}

auto Application::add_scene(std::unique_ptr<zc::Scene> scene) -> void {
    scenes.push_back(std::move(scene));
}

auto Application::switch_scene(std::size_t index) -> void {
    current_scene_index = std::clamp(index, std::size_t{0}, scenes.size() - 1);
}

auto Application::create_scenes() -> void {
    auto vs = zg::VertexShader::create("shaders/transCam.vert");
    auto fs = zg::FragmentShader::create("shaders/transformation.frag");
    auto fs_solid = zg::FragmentShader::create("shaders/solidColor.frag");

    if (!fs) {
        std::println(stderr, "{}", fs.error());
        return;
    }
    if (!vs) {
        std::println(stderr, "{}", vs.error());
        return;
    }
    if (!fs_solid) {
        std::println(stderr, "{}", fs_solid.error());
        return;
    }

    auto shader_program = std::make_shared<zg::ShaderProgram>(*vs, *fs);
    auto shader_program_2 = std::make_shared<zg::ShaderProgram>(*vs, *fs_solid);

    // Scene 1
    auto scene1 = SceneBuilder::create_triangles_scene(shader_program);
    this->add_scene(std::move(scene1));

    // Scene 2
    auto scene2 = SceneBuilder::create_ball_scene(shader_program);
    this->add_scene(std::move(scene2));

    // Scene 3 OBJECTS
    auto scene3 = SceneBuilder::create_basic_forest_scene(
        shader_program, shader_program_2
    );
    this->add_scene(std::move(scene3));

    // Scene 4 OBJECTS
    auto scene4 = SceneBuilder::create_login_scene(shader_program);
    this->add_scene(std::move(scene4));

    // SCENE 5
    auto scene5 = SceneBuilder::create_solar_scene(shader_program);
    this->add_scene(std::move(scene5));

    // SCENE 6 - Slenderman
    auto scene6 = SceneBuilder::create_slenderman_scene(shader_program);
    this->add_scene(std::move(scene6));

    // SCENE 7 - clock
    auto scene7 = SceneBuilder::create_forest_clock_scene(shader_program);
    this->add_scene(std::move(scene7));

    this->current_scene_index = this->scenes.size() - 1;
}

auto Application::run() -> void {
    Renderer::setup_rendering();

    while (!glfwWindowShouldClose(window->get())) {
        Renderer::setup_frame();

        Renderer::render(*this->scenes.at(this->current_scene_index));

        glfwSwapBuffers(window->get());
        glfwPollEvents();
    }
}
} // namespace zpg::core
