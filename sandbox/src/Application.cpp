#include "Application.hpp"
#include "models/bushes.h"
#include "models/earth.h"
#include "models/moon.h"
#include "models/sphere.h"
#include "models/sun.h"
#include "models/suzi_flat.h"
#include "models/text.h"
#include "models/tree.h"
#include "zpg/core/GFObserver.hpp"
#include "zpg/core/Renderer.hpp"
#include "zpg/core/Scene.hpp"
#include "zpg/graphics/DrawableObject.hpp"
#include "zpg/graphics/Group.hpp"
#include "zpg/graphics/Model.hpp"
#include "zpg/graphics/Rotation.hpp"
#include "zpg/graphics/RotationRepeat.hpp"
#include "zpg/graphics/Scale.hpp"
#include "zpg/graphics/Shader.hpp"
#include "zpg/graphics/ShaderProgram.hpp"
#include "zpg/graphics/Translation.hpp"
#include <algorithm>
#include <array>
#include <cstddef>
#include <cstdlib>
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

    // Scene 1 OBJECTS
    {
        auto scene1 = std::make_unique<zc::Scene>();
        scene1->add_shader_program(shader_program);

        // Triangle A: matrix S * T * R
        // Triangle B: matrix S * R * T
        {
            constexpr std::array<float, 18> POINTS_TRIANGLE_A{
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
            constexpr std::array<float, 18> POINTS_TRIANGLE_B{
              -0.5f,
              -0.5f,
              0.0f,
              0.0f,
              1.0f,
              1.0f,
              0.5f,
              -0.5f,
              0.0f,
              1.0f,
              0.0f,
              1.0f,
              0.0f,
              0.5f,
              0.0f,
              1.0f,
              1.0f,
              0.0f
            };

            auto triangle_model_a
                = zg::Model::create<zg::VertexLayout::PosColor>(
                      std::span<const float>{POINTS_TRIANGLE_A}
                )
                      .value();
            auto triangle_model_b
                = zg::Model::create<zg::VertexLayout::PosColor>(
                      std::span<const float>{POINTS_TRIANGLE_B}
                )
                      .value();

            // A: T applied first, then R (rotate around world origin).
            auto triangle_object_a = std::make_unique<zg::DrawableObject>(
                std::move(triangle_model_a), shader_program
            );
            triangle_object_a->add_transformation(
                std::make_unique<zg::Scale>(0.8f)
            );
            triangle_object_a->add_transformation(
                std::make_unique<zg::Translation>(glm::vec3{0.45f, -0.45f, 0.f})
            );
            triangle_object_a->add_transformation(
                std::make_unique<zg::Rotation>(glm::vec3{0.f, 0.f, 1.f}, 90.0f)
            );
            scene1->add_game_object(std::move(triangle_object_a));

            // B: R applied first (spin in place), then T.
            auto triangle_object_b = std::make_unique<zg::DrawableObject>(
                std::move(triangle_model_b), shader_program
            );
            triangle_object_b->add_transformation(
                std::make_unique<zg::Scale>(0.8f)
            );
            triangle_object_b->add_transformation(
                std::make_unique<zg::Rotation>(glm::vec3{0.f, 0.f, 1.f}, 90.0f)
            );
            triangle_object_b->add_transformation(
                std::make_unique<zg::Translation>(glm::vec3{0.45f, -0.45f, 0.f})
            );
            scene1->add_game_object(std::move(triangle_object_b));
        }

        // LOGIN OBJECT
        {
            auto login_model = zg::Model::create<zg::VertexLayout::PosNormal>(
                                   std::span<const float>{points}
            )
                                   .value();
            auto login_object = std::make_unique<zg::DrawableObject>(
                std::move(login_model), shader_program
            );
            login_object->add_transformation(
                std::make_unique<zg::Translation>(glm::vec3{0.81f, -0.89f, 0.f})
            );
            login_object->add_transformation(
                std::make_unique<zg::Rotation>(glm::vec3{0.f, 1.f, 0.f}, 20.0f)
            );
            login_object->add_transformation(
                std::make_unique<zg::Scale>(0.15f)
            );
            scene1->add_game_object(std::move(login_object));
        }

        this->add_scene(std::move(scene1));
    }

    // Scene 2 OBJECTS
    {
        auto scene2 = std::make_unique<zc::Scene>();
        scene2->add_shader_program(shader_program);

        // LOGIN OBJECT
        {
            auto login_model = zg::Model::create<zg::VertexLayout::PosNormal>(
                                   std::span<const float>{points}
            )
                                   .value();
            auto login_object = std::make_unique<zg::DrawableObject>(
                std::move(login_model), shader_program
            );
            login_object->add_transformation(
                std::make_unique<zg::Translation>(glm::vec3{0.81f, -0.89f, 0.f})
            );
            login_object->add_transformation(
                std::make_unique<zg::Rotation>(glm::vec3{0.f, 1.f, 0.f}, 20.0f)
            );
            login_object->add_transformation(
                std::make_unique<zg::Scale>(0.15f)
            );
            scene2->add_game_object(std::move(login_object));
        }

        // SPHERE OBJECT
        {
            auto sphere_model = zg::Model::create<zg::VertexLayout::PosColor>(
                                    std::span<const float>{sphere}
            )
                                    .value();
            auto sphere_object = std::make_unique<zg::DrawableObject>(
                std::move(sphere_model), shader_program
            );
            sphere_object->add_transformation(
                std::make_unique<zg::Translation>(glm::vec3{0.f, 0.1f, 0.f})
            );
            sphere_object->add_transformation(
                std::make_unique<zg::Rotation>(glm::vec3{0.f, 1.f, 0.f}, 45.0f)
            );
            sphere_object->add_transformation(
                std::make_unique<zg::Scale>(0.2f)
            );
            scene2->add_game_object(std::move(sphere_object));
        }

        this->add_scene(std::move(scene2));
    }

    // Scene 3 OBJECTS
    {
        auto scene3 = std::make_unique<zc::Scene>();
        scene3->add_shader_program(shader_program);

        // LOGIN OBJECT
        {
            auto login_model = zg::Model::create<zg::VertexLayout::PosNormal>(
                                   std::span<const float>{points}
            )
                                   .value();
            auto login_object = std::make_unique<zg::DrawableObject>(
                std::move(login_model), shader_program
            );
            login_object->add_transformation(
                std::make_unique<zg::Translation>(glm::vec3{0.81f, -0.89f, 0.f})
            );
            login_object->add_transformation(
                std::make_unique<zg::Rotation>(glm::vec3{0.f, 1.f, 0.f}, 20.0f)
            );
            login_object->add_transformation(
                std::make_unique<zg::Scale>(0.15f)
            );
            scene3->add_game_object(std::move(login_object));
        }

        {
            auto sun_model = zg::Model::create<zg::VertexLayout::PosColor>(
                                 std::span<const float>{sphere}
            )
                                 .value();
            auto sun_object = std::make_unique<zg::DrawableObject>(
                std::move(sun_model), shader_program_2
            );
            sun_object->add_transformation(
                std::make_unique<zg::Translation>(glm::vec3{0.f, 0.62f, 0.95f})
            );
            sun_object->add_transformation(std::make_unique<zg::Scale>(0.22f));
            shader_program_2->set_shader_program();
            const auto location
                = shader_program_2->get_uniform_location("fragmentColor");
            shader_program_2->set_uniform(location, 1.f, 1.f, 0.f);
            shader_program_2->unset_shader_program();
            scene3->add_game_object(std::move(sun_object));
        }

        // Each tree is paired with a bush placed at the same random spot
        {
            srand(777);

            constexpr auto RANDOM_BETWEEN
                = [](const float min, const float max) -> float {
                return min
                     + (static_cast<float>(rand())
                        / (static_cast<float>(RAND_MAX) / (max - min)));
            };

            for (std::size_t i = 0; i < 12; ++i) {
                const auto base_position = glm::vec3{
                  RANDOM_BETWEEN(-0.95f, 0.95f),
                  RANDOM_BETWEEN(-0.85f, -0.02f),
                  RANDOM_BETWEEN(0.05f, 0.75f)
                };

                // TREE at the base position
                {
                    auto tree_model
                        = zg::Model::create<zg::VertexLayout::PosColor>(
                              std::span<const float>{tree}
                        )
                              .value();
                    auto tree_object = std::make_unique<zg::DrawableObject>(
                        std::move(tree_model), shader_program
                    );
                    tree_object->add_transformation(
                        std::make_unique<zg::Translation>(base_position)
                    );
                    tree_object->add_transformation(
                        std::make_unique<zg::Rotation>(
                            glm::vec3{0.f, 1.f, 0.f}, RANDOM_BETWEEN(0.f, 360.f)
                        )
                    );
                    tree_object->add_transformation(
                        std::make_unique<zg::Scale>(
                            0.08f * RANDOM_BETWEEN(0.8f, 1.2f)
                        )
                    );
                    scene3->add_game_object(std::move(tree_object));
                }

                // BUSH next to the tree (same spot + small random offset)
                {
                    auto bush_model
                        = zg::Model::create<zg::VertexLayout::PosColor>(
                              std::span<const float>{bushes}
                        )
                              .value();
                    auto bush_object = std::make_unique<zg::DrawableObject>(
                        std::move(bush_model), shader_program
                    );
                    bush_object->add_transformation(
                        std::make_unique<zg::Translation>(base_position)
                    );
                    bush_object->add_transformation(
                        std::make_unique<zg::Translation>(glm::vec3{
                          RANDOM_BETWEEN(-0.08f, 0.08f),
                          0.f,
                          RANDOM_BETWEEN(-0.08f, 0.08f)
                        })
                    );
                    bush_object->add_transformation(
                        std::make_unique<zg::Rotation>(
                            glm::vec3{0.f, 1.f, 0.f}, RANDOM_BETWEEN(0.f, 360.f)
                        )
                    );
                    bush_object->add_transformation(
                        std::make_unique<zg::Scale>(
                            0.25f * RANDOM_BETWEEN(0.8f, 1.2f)
                        )
                    );
                    scene3->add_game_object(std::move(bush_object));
                }
            }
        }
        this->add_scene(std::move(scene3));
    }

    // Scene 4 OBJECTS
    {
        auto scene4 = std::make_unique<zc::Scene>();
        scene4->add_shader_program(shader_program);

        // LOGIN OBJECT
        {
            auto login_model = zg::Model::create<zg::VertexLayout::PosNormal>(
                                   std::span<const float>{points}
            )
                                   .value();
            auto login_object = std::make_unique<zg::DrawableObject>(
                std::move(login_model), shader_program
            );
            login_object->add_transformation(
                std::make_unique<zg::Rotation>(glm::vec3{0.f, 1.f, 0.f}, 32.0f)
            );
            login_object->add_transformation(std::make_unique<zg::Scale>(0.5f));
            scene4->add_game_object(std::move(login_object));
        }

        // LOGIN OBJECT
        {
            auto login_model = zg::Model::create<zg::VertexLayout::PosNormal>(
                                   std::span<const float>{points}
            )
                                   .value();
            auto login_object = std::make_unique<zg::DrawableObject>(
                std::move(login_model), shader_program
            );
            login_object->add_transformation(
                std::make_unique<zg::Translation>(glm::vec3{0.81f, -0.89f, 0.f})
            );
            login_object->add_transformation(
                std::make_unique<zg::Rotation>(glm::vec3{0.f, 1.f, 0.f}, 20.0f)
            );
            login_object->add_transformation(
                std::make_unique<zg::Scale>(0.15f)
            );
            scene4->add_game_object(std::move(login_object));
        }

        this->add_scene(std::move(scene4));
    }

    auto scene5 = std::make_unique<zc::Scene>();

    scene5->add_shader_program(shader_program);

    const auto spin = [](
                          const float degrees_per_second
                      ) -> std::unique_ptr<zg::RotationRepeat> {
        return std::make_unique<zg::RotationRepeat>(
            glm::vec3{0.f, 1.f, 0.f}, degrees_per_second
        );
    };

    auto solar_system = std::make_unique<zg::Group>();
    solar_system->set_transform(
        std::make_shared<zg::RotationRepeat>(glm::vec3{0.f, 1.f, 0.f}, 1.0f)
    );

    // SUN
    {
        auto sun_group = std::make_unique<zg::Group>();
        auto sun_model
            = zg::Model::create<zg::VertexLayout::PosColorNormal>(sun).value();
        auto sun_object = std::make_unique<zg::DrawableObject>(
            std::move(sun_model), shader_program
        );
        sun_object->add_transformation(std::make_unique<zg::Scale>(0.3f));
        sun_object->add_transformation(spin(15.f));
        sun_group->add(std::move(sun_object));
        solar_system->add(std::move(sun_group));
    }

    // EARTH
    {
        auto earth_group = std::make_unique<zg::Group>();
        earth_group->set_transform(
            std::make_shared<zg::Translation>(glm::vec3{0.75f, 0.f, 0.f})
        );

        auto earth_model
            = zg::Model::create<zg::VertexLayout::PosColorNormal>(earth)
                  .value();
        auto earth_object = std::make_unique<zg::DrawableObject>(
            std::move(earth_model), shader_program
        );
        earth_object->add_transformation(std::make_unique<zg::Scale>(0.08f));
        earth_object->add_transformation(spin(25.f));
        earth_group->add(std::move(earth_object));

        // MOON ORBIT rotates faster than the whole system -> nested groups
        {
            auto moon_orbit = std::make_unique<zg::Group>();
            moon_orbit->set_transform(
                std::make_shared<zg::RotationRepeat>(
                    glm::vec3{0.f, 1.f, 0.f}, 6.0f
                )
            );

            auto moon_group = std::make_unique<zg::Group>();
            moon_group->set_transform(
                std::make_shared<zg::Translation>(glm::vec3{0.15f, 0.f, 0.f})
            );
            auto moon_model
                = zg::Model::create<zg::VertexLayout::PosColorNormal>(moon)
                      .value();
            auto moon_object = std::make_unique<zg::DrawableObject>(
                std::move(moon_model), shader_program
            );
            moon_object->add_transformation(
                std::make_unique<zg::Scale>(0.008f)
            );
            moon_object->add_transformation(spin(40.f));
            moon_group->add(std::move(moon_object));

            moon_orbit->add(std::move(moon_group));
            earth_group->add(std::move(moon_orbit));
        }

        solar_system->add(std::move(earth_group));
    }

    scene5->add_node(std::move(solar_system));

    // LOGIN OBJECT
    {
        auto login_model = zg::Model::create<zg::VertexLayout::PosNormal>(
                               std::span<const float>{points}
        )
                               .value();
        auto login_object = std::make_unique<zg::DrawableObject>(
            std::move(login_model), shader_program
        );
        login_object->add_transformation(
            std::make_unique<zg::Translation>(glm::vec3{0.81f, -0.89f, 0.f})
        );
        login_object->add_transformation(
            std::make_unique<zg::Rotation>(glm::vec3{0.f, 1.f, 0.f}, 20.0f)
        );
        login_object->add_transformation(std::make_unique<zg::Scale>(0.15f));
        scene5->add_game_object(std::move(login_object));
    }

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
