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
#include <span>
#include <vector>

namespace {

// Firefly orbit
constexpr float FIREFLY_SPEED = 24.f;
constexpr float FIREFLY_RADIUS = 3.f;
constexpr float FIREFLY_HEIGHT = 1.6f;

[[nodiscard]] auto make_colored_tree() -> std::vector<float> {
    constexpr std::size_t SOURCE_STRIDE = 6;
    constexpr std::size_t TRUNK_VERTICES = 36'444;
    const std::span<const float> source{tree};

    std::vector<float> result;
    result.reserve(source.size() / SOURCE_STRIDE * 9);

    for (std::size_t i = 0; i < source.size() / SOURCE_STRIDE; ++i) {
        const auto offset = i * SOURCE_STRIDE;
        const auto color = i < TRUNK_VERTICES ? glm::vec3{0.36f, 0.20f, 0.08f}
                                              : glm::vec3{0.07f, 0.38f, 0.12f};

        result.insert(
            result.end(),
            {source[offset],
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

[[nodiscard]] auto make_colored_bush() -> std::vector<float> {
    constexpr std::size_t SOURCE_STRIDE = 6;
    constexpr glm::vec3 COLOR{0.04f, 0.28f, 0.08f};
    const std::span<const float> source{bushes};

    std::vector<float> result;
    result.reserve(source.size() / SOURCE_STRIDE * 9);

    for (std::size_t i = 0; i < source.size(); i += SOURCE_STRIDE) {
        result.insert(
            result.end(),
            {source[i],
             source[i + 1],
             source[i + 2],
             COLOR.r,
             COLOR.g,
             COLOR.b,
             source[i + 3],
             source[i + 4],
             source[i + 5]}
        );
    }

    return result;
}

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
    scene->add_object<zg::VertexLayout::PosNormal>(PLAIN, ground_program)
        .add_transformation(zg::Translation{{0.f, -2.1f, 0.f}})
        .add_transformation(zg::Scale{100.f});

    // FIREFLY BALL
    auto& orbit
        = scene->add_group(zg::RotationRepeat{{0.f, 1.f, 0.f}, FIREFLY_SPEED});
    orbit.add_object<zg::VertexLayout::PosNormal>(sphere, firefly_program)
        .add_transformation(
            zg::Translation{{FIREFLY_RADIUS, FIREFLY_HEIGHT, 0.f}}
        )
        .add_transformation(zg::Scale{0.08f});

    // FOREST
    auto points
        = zpg::utils::poisson_disk(666, {-30, -30}, glm::vec2{30, 30}, 5.5f);
    auto bush_points
        = zpg::utils::poisson_disk(667, {-30, -30}, glm::vec2{30, 30}, 4.f);

    const auto tree_model = make_colored_tree();
    const auto bush_model = make_colored_bush();

    std::mt19937 random{666};
    std::uniform_real_distribution<float> rotation{0.f, 360.f};
    std::uniform_real_distribution<float> scale{0.8f, 1.2f};

    auto& forest_group = scene->add_group();
    auto& bush_group = scene->add_group();

    for (glm::vec2 p : std::views::take(points, 76)) {
        auto& tree_group = forest_group.add_group();
        const float tree_scale = scale(random);

        tree_group.add_transformation(zg::Translation{{p.x, -1.f, p.y}});

        tree_group
            .add_object<zg::VertexLayout::PosColorNormal>(
                tree_model, forest_program
            )
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

        bush_group
            .add_object<zg::VertexLayout::PosColorNormal>(
                bush_model, forest_program
            )
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
