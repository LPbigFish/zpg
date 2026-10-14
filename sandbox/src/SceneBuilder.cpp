#include "SceneBuilder.hpp"
#include "models/SM_Model.hpp"
#include "models/bushes.h"
#include "models/clock.h"
#include "models/earth.h"
#include "models/moon.h"
#include "models/sphere.h"
#include "models/sun.h"
#include "models/text.h"
#include "models/tree.h"
#include "zpg/core/Scene.hpp"
#include "zpg/graphics/DrawableObject.hpp"
#include "zpg/graphics/Model.hpp"
#include "zpg/graphics/RotationRepeat.hpp"
#include "zpg/graphics/ShaderProgram.hpp"
#include "zpg/graphics/VertexArray.hpp"
#include <memory>
#include <utility>

namespace {

[[nodiscard]] constexpr auto make_colored_tree() -> std::vector<float> {
    constexpr std::size_t SOURCE_STRIDE = 6;
    constexpr std::size_t TRUNK_VERTICES = 36'444;

    const std::span<const float> source{tree};

    std::vector<float> result;
    result.reserve(source.size() / SOURCE_STRIDE * 9);

    for (std::size_t i = 0; i < source.size() / SOURCE_STRIDE; ++i) {
        const auto offset = i * SOURCE_STRIDE;

        const auto color = i < TRUNK_VERTICES
                             ? glm::vec3{0.36f, 0.20f, 0.08f}  // Brown bark
                             : glm::vec3{0.07f, 0.38f, 0.12f}; // Green leaves

        result.insert(
            result.end(),
            {source[offset + 0],
             source[offset + 1],
             source[offset + 2],

             color[0],
             color[1],
             color[2],

             source[offset + 3],
             source[offset + 4],
             source[offset + 5]}
        );
    }

    return result;
}
} // namespace

auto SceneBuilder::create_triangles_scene(
    const std::shared_ptr<zg::ShaderProgram>& shader_program
) -> std::unique_ptr<zc::Scene> {
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

        return scene1;
    }
}

auto SceneBuilder::create_ball_scene(
    const std::shared_ptr<zg::ShaderProgram>& shader_program
) -> std::unique_ptr<zc::Scene> {
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
        login_object->add_transformation(std::make_unique<zg::Scale>(0.15f));
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
        sphere_object->add_transformation(std::make_unique<zg::Scale>(0.2f));
        scene2->add_game_object(std::move(sphere_object));
    }

    return scene2;
}

auto SceneBuilder::create_basic_forest_scene(
    const std::shared_ptr<zg::ShaderProgram>& shader_program,
    const std::shared_ptr<zg::ShaderProgram>& shader_program_2
) -> std::unique_ptr<zc::Scene> {
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
        login_object->add_transformation(std::make_unique<zg::Scale>(0.15f));
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

        auto tree_m = make_colored_tree();

        for (std::size_t i = 0; i < 12; ++i) {
            const auto base_position = glm::vec3{
              RANDOM_BETWEEN(-0.95f, 0.95f),
              RANDOM_BETWEEN(-0.85f, -0.02f),
              RANDOM_BETWEEN(0.05f, 0.75f)
            };

            // TREE at the base position
            {
                auto tree_model
                    = zg::Model::create<zg::VertexLayout::PosColorNormal>(
                          std::span<const float>{tree_m}
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
                auto bush_model = zg::Model::create<zg::VertexLayout::PosColor>(
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
    return scene3;
}

auto SceneBuilder::create_login_scene(
    const std::shared_ptr<zg::ShaderProgram>& shader_program
) -> std::unique_ptr<zc::Scene> {
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
        login_object->add_transformation(std::make_unique<zg::Scale>(0.15f));
        scene4->add_game_object(std::move(login_object));
    }

    return scene4;
}

auto SceneBuilder::create_solar_scene(
    const std::shared_ptr<zg::ShaderProgram>& shader_program
) -> std::unique_ptr<zc::Scene> {
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

    return scene5;
}

auto SceneBuilder::create_slenderman_scene(
    const std::shared_ptr<zg::ShaderProgram>& shader_program
) -> std::unique_ptr<zc::Scene> {
    auto scene6 = std::make_unique<zc::Scene>();
    scene6->add_shader_program(shader_program);

    {
        auto slenderman
            = zg::Model::create<zg::VertexLayout::PosColorNormal>(SM_Model)
                  .value();
        auto slender_object = std::make_unique<zg::DrawableObject>(
            std::move(slenderman), shader_program
        );
        slender_object->add_transformation(
            zg::Translation(glm::vec3{-0.5f, -0.1f, 0.f})
        );
        slender_object->add_transformation(
            zg::Rotation(glm::vec3{0.f, 1.f, 0.f}, -90.f)
        );
        slender_object->add_transformation(zg::Scale(0.7f));
        slender_object->add_transformation(
            zg::Rotation(glm::vec3{0.f, 1.f, 0.f}, -30.f)
        );

        scene6->add_game_object(std::move(slender_object));
    }

    return scene6;
}

auto SceneBuilder::create_forest_clock_scene(
    const std::shared_ptr<zg::ShaderProgram>& shader_program
) -> std::unique_ptr<zc::Scene> {
    auto scene7 = std::make_unique<zc::Scene>();

    auto clock_model
        = zg::Model::create<zg::VertexLayout::PosNormalColor>(clockModel)
              .value();
    auto clock_obj = std::make_unique<zg::DrawableObject>(
        std::move(clock_model), shader_program
    );

    scene7->add_game_object(std::move(clock_obj));

    return scene7;
}
