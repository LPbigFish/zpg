#include "Application.hpp"
#include "models/bushes.h"
#include "models/sphere.h"
#include "models/suzi_flat.h"
#include "models/text.h"
#include "models/tree.h"
#include "zpg/core/GFObserver.hpp"
#include "zpg/graphics/Shader.hpp"
#include "zpg/graphics/ShaderProgram.hpp"
#include <algorithm>
#include <functional>
#include <glm/gtc/type_ptr.hpp>
#include <stdexcept>
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
    glViewport(0, 0, width, height);

    glfwSetFramebufferSizeCallback(
        window->get(),
        // NOLINTNEXTLINE
        [](GLFWwindow*, int width, int height) -> void {
            glViewport(0, 0, width, height);
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
    auto vs = zg::VertexShader::create("shaders/transformation.vert");
    auto fs = zg::FragmentShader::create("shaders/transformation.frag");
    auto fs2 = zg::FragmentShader::create("shaders/solidColor.frag");

    if (!vs) {
        std::println(stderr, "{}", vs.error());
    }
    if (!fs) {
        std::println(stderr, "{}", fs.error());
    }
    if (!fs2) {
        std::println(stderr, "{}", fs.error());
    }

    auto shader_program = std::make_shared<zg::ShaderProgram>(*vs, *fs);
    auto shader_program_2 = std::make_shared<zg::ShaderProgram>(*vs, *fs2);

    auto scene1 = std::make_unique<zc::Scene>();
    auto scene2 = std::make_unique<zc::Scene>();
    auto scene3 = std::make_unique<zc::Scene>();
    auto scene4 = std::make_unique<zc::Scene>();

    scene1->add_shader_program(shader_program);
    scene2->add_shader_program(shader_program);
    scene3->add_shader_program(shader_program);
    scene3->add_shader_program(shader_program_2);
    scene4->add_shader_program(shader_program);

    // Scene 1 OBJECTS
    {
        // TRIANGLE OBJECT
        {
            std::array<float, 18> points_triangle{
              -0.5f,
              -0.5f,
              0.0f,
              1.0f,
              0.0f,
              0.0f,
              0.5f,
              -0.5f,
              0.0f,
              0.0f,
              1.0f,
              0.0f,
              0.0f,
              0.5f,
              0.0f,
              0.0f,
              0.0f,
              1.0f
            };
            auto triangle_model
                = zg::Model{std::span<const float>{points_triangle}};
            auto triangle_object = std::make_unique<zg::DrawableObject>(
                std::move(triangle_model), shader_program
            );
            triangle_object->set_offset({0.0f, 0.0f, 0.0f});
            triangle_object->set_scale(glm::vec3{1.0f, 1.0f, 1.0f});
            triangle_object->set_angle(45);
            scene1->add_game_object(std::move(triangle_object));
        }

        // LOGIN OBJECT
        {
            auto login_model = zg::Model{std::span<const float>{points}};
            auto login_object = std::make_unique<zg::DrawableObject>(
                std::move(login_model), shader_program
            );
            login_object->set_offset({0.81f, -0.89f, 0.0f});
            login_object->set_scale(glm::vec3{0.15f, 0.15f, 0.15f});
            login_object->set_angle(20);
            scene1->add_game_object(std::move(login_object));
        }

        this->add_scene(std::move(scene1));
    }

    // Scene 2 OBJECTS
    {
        // LOGIN OBJECT
        {
            auto login_model = zg::Model{std::span<const float>{points}};
            auto login_object = std::make_unique<zg::DrawableObject>(
                std::move(login_model), shader_program
            );
            login_object->set_offset({0.81f, -0.89f, 0.0f});
            login_object->set_scale(glm::vec3{0.15f, 0.15f, 0.15f});
            login_object->set_angle(20);
            scene2->add_game_object(std::move(login_object));
        }

        // SPHERE OBJECT
        {
            auto sphere_model = zg::Model{std::span<const float>{sphere}};
            auto sphere_object = std::make_unique<zg::DrawableObject>(
                std::move(sphere_model), shader_program
            );
            sphere_object->set_offset({0.0f, 0.1f, 0.0f});
            sphere_object->set_scale(glm::vec3{0.2f, 0.2f, 0.2f});
            sphere_object->set_angle(45);
            scene2->add_game_object(std::move(sphere_object));
        }

        this->add_scene(std::move(scene2));
    }

    // Scene 3 OBJECTS
    {
        // LOGIN OBJECT
        {
            auto login_model = zg::Model{std::span<const float>{points}};
            auto login_object = std::make_unique<zg::DrawableObject>(
                std::move(login_model), shader_program
            );
            login_object->set_offset({0.81f, -0.89f, 0.0f});
            login_object->set_scale(glm::vec3{0.15f, 0.15f, 0.15f});
            login_object->set_angle(20);
            scene3->add_game_object(std::move(login_object));
        }

        // Fixed bushes
        {
            const std::array bush_positions{
              glm::vec3{-0.82f, -0.72f, 0.65f},
              glm::vec3{0.36f, -0.78f, 0.72f},
              glm::vec3{0.79f, -0.61f, 0.52f},
              glm::vec3{-0.24f, -0.58f, 0.45f},
              glm::vec3{-0.62f, -0.36f, 0.38f},
              glm::vec3{0.15f, -0.42f, 0.55f},
              glm::vec3{0.62f, -0.28f, 0.33f},
              glm::vec3{-0.85f, -0.12f, 0.25f},
              glm::vec3{-0.28f, -0.18f, 0.20f},
              glm::vec3{0.38f, -0.08f, 0.15f},
              glm::vec3{-0.92f, -0.50f, 0.12f},
              glm::vec3{0.88f, -0.04f, 0.10f}
            };

            for (const auto& position : bush_positions) {
                auto bush_model = zg::Model{std::span<const float>{bushes}};
                auto bush_object = std::make_unique<zg::DrawableObject>(
                    std::move(bush_model), shader_program
                );
                bush_object->set_offset(position);
                bush_object->set_scale(glm::vec3{0.25f, 0.25f, 0.25f});
                bush_object->set_angle(90);
                scene3->add_game_object(std::move(bush_object));
            }
        }

        // Fixed trees
        {
            const std::array tree_positions{
              glm::vec3{-0.56f, -0.80f, 0.68f},
              glm::vec3{0.78f, -0.83f, 0.62f},
              glm::vec3{0.06f, -0.69f, 0.58f},
              glm::vec3{-0.88f, -0.48f, 0.50f},
              glm::vec3{0.49f, -0.49f, 0.46f},
              glm::vec3{-0.17f, -0.36f, 0.42f},
              glm::vec3{0.83f, -0.22f, 0.36f},
              glm::vec3{-0.58f, -0.20f, 0.30f},
              glm::vec3{0.12f, -0.10f, 0.24f},
              glm::vec3{-0.06f, -0.50f, 0.18f},
              glm::vec3{0.70f, -0.65f, 0.14f},
              glm::vec3{-0.42f, -0.62f, 0.12f}
            };

            for (const auto& position : tree_positions) {
                auto tree_model = zg::Model{std::span<const float>{tree}};
                auto tree_object = std::make_unique<zg::DrawableObject>(
                    std::move(tree_model), shader_program
                );
                tree_object->set_offset(position);
                tree_object->set_scale(glm::vec3{0.08f, 0.08f, 0.08f});
                tree_object->set_angle(90);
                scene3->add_game_object(std::move(tree_object));
            }
        }

        // Sun in the middle
        {
            auto sun_model = zg::Model{std::span<const float>{sphere}};
            auto sun_object = std::make_unique<zg::DrawableObject>(
                std::move(sun_model), shader_program_2
            );
            sun_object->set_offset({0.0f, 0.62f, -0.95f});
            sun_object->set_scale(glm::vec3{0.22f, 0.22f, 0.22f});
            shader_program_2->set_shader_program();
            sun_object->set_uniform("fragmentColor", 1.f, 1.f, 0.f);
            shader_program_2->unset_shader_program();
            scene3->add_game_object(std::move(sun_object));
        }

        this->add_scene(std::move(scene3));
    }

    // Scene 4 OBJECTS
    {
        // LOGIN OBJECT
        {
            auto login_model = zg::Model{std::span<const float>{points}};
            auto login_object = std::make_unique<zg::DrawableObject>(
                std::move(login_model), shader_program
            );
            login_object->set_offset({0.0f, 0.0f, 0.0f});
            login_object->set_scale(glm::vec3{0.5f, 0.5f, 0.5f});
            login_object->set_angle(32);
            /*
            login_object->set_draw_callback(
                [](zg::DrawableObject& self) -> void {
                    auto angle = self.get_angle();
                    self.set_angle(static_cast<std::int16_t>(angle + 1));
                }
            );
            */
            scene4->add_game_object(std::move(login_object));
        }

        // LOGIN OBJECT
        {
            auto login_model = zg::Model{std::span<const float>{points}};
            auto login_object = std::make_unique<zg::DrawableObject>(
                std::move(login_model), shader_program
            );
            login_object->set_offset({0.81f, -0.89f, 0.0f});
            login_object->set_scale(glm::vec3{0.15f, 0.15f, 0.15f});
            login_object->set_angle(20);
            scene4->add_game_object(std::move(login_object));
        }

        this->add_scene(std::move(scene4));
    }
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
    glEnable(GL_DEPTH_TEST);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    while (!glfwWindowShouldClose(window->get())) {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        if (!scenes.empty()) {
            scenes.at(current_scene_index)->render();
        }

        glfwSwapBuffers(window->get());
        glfwPollEvents();
    }
}
} // namespace zpg::core
