#include "../SceneBuilder.hpp"
#include "zpg/graphics/PointLight.hpp"
#include "zpg/graphics/RotationRepeat.hpp"
#include "zpg/graphics/Scale.hpp"
#include "zpg/graphics/Translation.hpp"
#include "zpg/graphics/VertexArray.hpp"
#include "zpg/utils/Time.hpp"
#include "zpg/utils/poissonDisk.hpp"
#include <cmath>
#include <glm/trigonometric.hpp>
#include <memory>
#include <random>
#include <ranges>

namespace {

// Firefly orbit
constexpr float FIREFLY_SPEED = 24.f;
constexpr float FIREFLY_RADIUS = 3.f;
constexpr float FIREFLY_HEIGHT = 1.6f;

} // namespace

auto SceneBuilder::create_night_forest_scene(
    const std::shared_ptr<zg::ShaderProgram>& forest_program,
    const std::shared_ptr<zg::ShaderProgram>& ground_program,
    const std::shared_ptr<zg::ShaderProgram>& firefly_program,
    const std::shared_ptr<zg::ShaderProgram>& fixed_shader_program
) -> std::unique_ptr<zc::Scene> {
    auto scene = std::make_unique<zc::Scene>();

    const zg::PointLight firefly{
      .position = {0.f, FIREFLY_HEIGHT, 0.f},
      .color = {1.f, 1.f, 0.5f},
      .intensity = 2.2f,
      .constant = 1.f,
      .linear = 0.35f,
      .quadratic = 0.10f
    };

    auto set_static_uniforms =
        [&firefly](const std::shared_ptr<zg::ShaderProgram>& program) -> void {
        program->set_shader_program();

        if (auto loc = program->get_uniform_location("lightColor"); loc != -1) {
            program->set_uniform(
                loc, firefly.color.r, firefly.color.g, firefly.color.b
            );
        }
        if (auto loc = program->get_uniform_location("intensity"); loc != -1) {
            program->set_uniform(loc, firefly.intensity);
        }
        if (auto loc = program->get_uniform_location("kc"); loc != -1) {
            program->set_uniform(loc, firefly.constant);
        }
        if (auto loc = program->get_uniform_location("kl"); loc != -1) {
            program->set_uniform(loc, firefly.linear);
        }
        if (auto loc = program->get_uniform_location("kq"); loc != -1) {
            program->set_uniform(loc, firefly.quadratic);
        }

        program->unset_shader_program();
    };

    set_static_uniforms(forest_program);
    set_static_uniforms(ground_program);

    ground_program->set_shader_program();
    ground_program->set_uniform(
        ground_program->get_uniform_location("color"), 0.05f, 0.11f, 0.04f
    );
    ground_program->unset_shader_program();

    // GROUND
    scene->add_object(model(ModelId::Plain), ground_program)
        .add_transformation(zg::Translation{{0.f, -2.1f, 0.f}})
        .add_transformation(zg::Scale{100.f});

    // FIREFLY BALL
    auto& orbit
        = scene->add_group(zg::RotationRepeat{{0.f, 1.f, 0.f}, FIREFLY_SPEED});
    orbit.add_object(model(ModelId::SphereNormal), firefly_program)
        .add_transformation(
            zg::Translation{{FIREFLY_RADIUS, FIREFLY_HEIGHT, 0.f}}
        )
        .add_transformation(zg::Scale{0.08f});

    // FOREST
    auto points
        = zpg::utils::poisson_disk(666, {-30, -30}, glm::vec2{30, 30}, 5.5f);
    auto bush_points
        = zpg::utils::poisson_disk(667, {-30, -30}, glm::vec2{30, 30}, 4.f);

    std::mt19937 random{666};
    std::uniform_real_distribution<float> rotation{0.f, 360.f};
    std::uniform_real_distribution<float> scale{0.8f, 1.2f};

    auto& forest_group = scene->add_group();
    auto& bush_group = scene->add_group();

    for (glm::vec2 p : std::views::take(points, 76)) {
        auto& tree_group = forest_group.add_group();
        const float tree_scale = scale(random);

        tree_group.add_transformation(zg::Translation{{p.x, -1.f, p.y}});

        tree_group.add_object(model(ModelId::TreeColored), forest_program)
            .add_transformation(zg::Translation{{0, -1, 0}})
            .add_transformation(zg::Rotation{{0.f, 1.f, 0.f}, rotation(random)})
            .add_transformation(
                zg::Scale{
                  {2.3f * tree_scale, 5.f * tree_scale, 2.3f * tree_scale}
                }
            );
    }

    for (glm::vec2 p : std::views::take(bush_points, 100)) {
        const float bush_scale = scale(random);

        bush_group.add_object(model(ModelId::BushColored), forest_program)
            .add_transformation(zg::Translation{{p.x, -2.f, p.y}})
            .add_transformation(zg::Scale{2.f * bush_scale})
            .add_transformation(zg::Rotation{{0, 1, 0}, rotation(random)});
    }

    scene->set_update([forest_program, ground_program](float) -> void {
        const float theta = glm::radians(FIREFLY_SPEED)
                          * zpg::utils::Time::get_elapsed_time();
        const glm::vec3 light_position{
          FIREFLY_RADIUS * std::cos(theta),
          FIREFLY_HEIGHT,
          -FIREFLY_RADIUS * std::sin(theta)
        };

        for (const auto& program : {forest_program, ground_program}) {
            program->set_shader_program();
            if (auto loc = program->get_uniform_location("lightPosition");
                loc != -1) {
                program->set_uniform(
                    loc, light_position.x, light_position.y, light_position.z
                );
            }
            program->unset_shader_program();
        }
    });

    scene->get_camera().set_position({0.f, 1.8f, 9.f});

    add_signature(scene, fixed_shader_program);

    return scene;
}
