#include "Application.hpp"
#include "models/bushes.h"
#include "models/earth.h"
#include "models/moon.h"
#include "models/sphere.h"
#include "models/sun.h"
#include "models/suzi_flat.h"
#include "models/tree.h"
#include "zpg/core/GFObserver.hpp"
#include "zpg/core/Renderer.hpp"
#include "zpg/core/Scene.hpp"
#include "zpg/graphics/DrawableObject.hpp"
#include "zpg/graphics/Group.hpp"
#include "zpg/graphics/Model.hpp"
#include "zpg/graphics/SceneNode.hpp"
#include "zpg/graphics/Shader.hpp"
#include "zpg/graphics/ShaderProgram.hpp"
#include <algorithm>
#include <cstddef>
#include <functional>
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
    auto vs = zg::VertexShader::create("shaders/trans.vert");
    auto fs = zg::FragmentShader::create("shaders/transformation.frag");

    if (!fs) {
        std::println(stderr, "{}", fs.error());
        return;
    }
    if (!vs) {
        std::println(stderr, "{}", vs.error());
        return;
    }

    auto good_transform_program = std::make_shared<zg::ShaderProgram>(*vs, *fs);

    const auto set_rotation = [](zg::SceneNode& object, float degrees) -> void {
        object.set_draw_callback(
            [clock = std::size_t{0},
             degrees](zg::SceneNode& node) mutable -> void {
                if (++clock == 3) {
                    clock = 0;
                    node.rotate_y(glm::radians(degrees));
                }
            }
        );
    };

    auto scene5 = std::make_unique<zc::Scene>();

    scene5->add_shader_program(good_transform_program);

    auto solar_system = std::make_unique<zg::Group>();
    solar_system->set_draw_callback(
        [clock = std::size_t{0}](zg::SceneNode& node) mutable -> void {
            if (++clock == 4) {
                clock = 0;
                node.rotate_z(glm::radians(0.05f));
            }
        }
    );

    auto sun_group = std::make_unique<zg::Group>();
    auto sun_model = zg::Model{sun};
    auto sun_object = std::make_unique<zg::DrawableObject>(
        std::move(sun_model), good_transform_program
    );
    sun_object->scale(0.3f);
    sun_object->rotate_x(glm::radians(90.0f));
    set_rotation(*sun_object, 0.5f);
    sun_group->add(std::move(sun_object));
    solar_system->add(std::move(sun_group));

    auto earth_group = std::make_unique<zg::Group>();
    earth_group->transform(glm::vec3{0.75f, 0.0f, 0.0f});

    auto earth_model = zg::Model{earth};
    auto earth_object = std::make_unique<zg::DrawableObject>(
        std::move(earth_model), good_transform_program
    );
    earth_object->scale(0.08f);
    earth_object->rotate_x(glm::radians(90.0f));
    set_rotation(*earth_object, 0.5f);
    earth_group->add(std::move(earth_object));

    auto moon_orbit = std::make_unique<zg::Group>();
    moon_orbit->set_draw_callback(
        [clock = std::size_t{0}](zg::SceneNode& node) mutable -> void {
            if (++clock == 8) {
                clock = 0;
                node.rotate_z(glm::radians(1.0f));
            }
        }
    );

    auto moon_group = std::make_unique<zg::Group>();
    moon_group->transform(glm::vec3{0.15f, 0.0f, 0.0f});
    auto moon_model = zg::Model{moon};
    auto moon_object = std::make_unique<zg::DrawableObject>(
        std::move(moon_model), good_transform_program
    );
    moon_object->scale(0.008f);
    moon_object->rotate_x(glm::radians(90.0f));
    set_rotation(*moon_object, 0.5f);
    moon_group->add(std::move(moon_object));

    moon_orbit->add(std::move(moon_group));
    earth_group->add(std::move(moon_orbit));
    solar_system->add(std::move(earth_group));
    scene5->add_node(std::move(solar_system));

    this->add_scene(std::move(scene5));
}

auto Application::run(const std::function<void()>& loop_program) -> void {
    glEnable(GL_DEPTH_TEST);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    while (!glfwWindowShouldClose(window->get())) {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        loop_program();

        glfwSwapBuffers(window->get());
        glfwPollEvents();
    }
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
