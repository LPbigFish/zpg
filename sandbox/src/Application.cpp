#include "Application.hpp"
#include "SceneBuilder.hpp"
#include "zpg/core/GFObserver.hpp"
#include "zpg/core/Renderer.hpp"
#include "zpg/core/Scene.hpp"
#include "zpg/graphics/Shader.hpp"
#include "zpg/graphics/ShaderProgram.hpp"
#include <cstddef>
#include <cstdio>
#include <cstdlib>
#include <glm/ext/quaternion_transform.hpp>
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

    Application::print_info();
}

auto Application::create_callbacks() -> void {
    namespace gfo = zpg::core::gfobserver;
    namespace events = gfo::events;

    auto& observer = gfo::GlfwObserver::instance();
    input_manager.subscribe(observer);

    // ERRORs
    observer.subscribe<events::Error>([](const events::Error& event) -> void {
        std::println(
            stderr, "error_callback [{}]: {}", event.error, event.description
        );
    });

    // KEY EVENTS
    observer.subscribe<events::KeyEvent>(
        [this](const events::KeyEvent& event) -> void {
            if (event.key > GLFW_KEY_0 && event.key <= GLFW_KEY_9
                && event.action == GLFW_PRESS) {
                auto index = static_cast<std::size_t>(event.key - GLFW_KEY_1);
                this->switch_scene(index);
                std::println("Switched to scene {}", index);
            }
        }
    );

    // FRAME BUFFER RESIZE
    observer.subscribe<events::FrameBufferSizeEvent>(
        [this](const events::FrameBufferSizeEvent& event) -> void {
            Renderer::set_viewport(event.width, event.height);

            if (!scenes.empty()) {
                scenes[current_scene_index]->get_camera().resize(
                    event.width, event.height
                );
            }
        }
    );
}

auto Application::add_scene(std::unique_ptr<zc::Scene> scene) -> void {
    scenes.push_back(std::move(scene));
}

auto Application::switch_scene(std::size_t index) -> void {
    if (index >= scenes.size()) {
        return;
    }

    current_scene_index = index;
    input_manager.reset_cursor();

    int width{};
    int height{};
    glfwGetFramebufferSize(window->get(), &width, &height);

    auto& scene = *scenes[current_scene_index];

    scene.get_camera().resize(width, height);
    scene.activate();
}

auto Application::create_scenes() -> void {
    auto vs = zg::VertexShader::create("shaders/transCam.vert");
    auto vs_fixed = zg::VertexShader::create("shaders/trans.vert");
    auto fs = zg::FragmentShader::create("shaders/transformation.frag");
    auto fs_solid = zg::FragmentShader::create("shaders/solidColor.frag");
    auto vs_clock = zg::VertexShader::create("shaders/clock.vert");
    auto fs_clock = zg::FragmentShader::create("shaders/clock.frag");

    auto fs_tree = zg::FragmentShader::create("shaders/tree.frag");
    auto vs_phong = zg::VertexShader::create("shaders/phong.vert");
    auto fs_phong = zg::FragmentShader::create("shaders/phong.frag");

    if (!vs) {
        std::println(stderr, "{}", vs.error());
        return;
    }
    if (!vs_fixed) {
        std::println(stderr, "{}", vs_fixed.error());
        return;
    }
    if (!fs) {
        std::println(stderr, "{}", fs.error());
        return;
    }
    if (!fs_solid) {
        std::println(stderr, "{}", fs_solid.error());
        return;
    }
    if (!vs_clock) {
        std::println(stderr, "{}", vs_clock.error());
        return;
    }
    if (!fs_clock) {
        std::println(stderr, "{}", fs_clock.error());
        return;
    }
    if (!fs_tree) {
        std::println(stderr, "{}", fs_tree.error());
        return;
    }

    auto shader_program = std::make_shared<zg::ShaderProgram>(*vs, *fs);
    auto fixed_shader_program
        = std::make_shared<zg::ShaderProgram>(*vs_fixed, *fs);
    auto tree_program = std::make_shared<zg::ShaderProgram>(*vs, *fs_tree);
    auto shader_program_2 = std::make_shared<zg::ShaderProgram>(*vs, *fs_solid);
    auto clock_shader_program
        = std::make_shared<zg::ShaderProgram>(*vs_clock, *fs_clock);

    auto phong_program
        = std::make_shared<zg::ShaderProgram>(*vs_phong, *fs_phong);

    // Scene 1
    auto scene1 = SceneBuilder::create_triangles_scene(fixed_shader_program);
    this->add_scene(std::move(scene1));

    // Scene 2
    auto scene2 = SceneBuilder::create_ball_scene(fixed_shader_program);
    this->add_scene(std::move(scene2));

    // SCENE 3
    auto scene3 = SceneBuilder::create_four_ball_scene(phong_program);
    this->add_scene(std::move(scene3));

    // Scene 4 OBJECTS
    auto scene4 = SceneBuilder::create_login_scene(fixed_shader_program);
    this->add_scene(std::move(scene4));

    // SCENE 5
    auto scene5 = SceneBuilder::create_solar_scene(
        shader_program, fixed_shader_program
    );
    this->add_scene(std::move(scene5));

    // SCENE 7 - clock
    auto scene7 = SceneBuilder::create_forest_clock_scene(
        shader_program, tree_program, fixed_shader_program, clock_shader_program
    );
    this->add_scene(std::move(scene7));

    this->current_scene_index = this->scenes.size() - 1;
}

auto Application::run() -> void {
    Renderer::setup_rendering();

    double last_time = glfwGetTime();
    while (!glfwWindowShouldClose(window->get())) {
        glfwPollEvents();

        const double now = glfwGetTime();
        const auto dt = static_cast<float>(now - last_time);
        last_time = now;

        auto& scene = *scenes.at(current_scene_index);
        input_manager.update(scene.get_camera(), dt);
        scene.update(dt);

        Renderer::setup_frame();

        Renderer::render(scene);

        glfwSwapBuffers(window->get());
    }
}
} // namespace zpg::core
