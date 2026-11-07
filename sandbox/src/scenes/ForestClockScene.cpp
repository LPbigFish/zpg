#include "SceneBuilder.hpp"
#include "zpg/graphics/Rotation.hpp"
#include "zpg/graphics/Scale.hpp"
#include "zpg/graphics/Translation.hpp"
#include "zpg/graphics/VertexArray.hpp"
#include "zpg/utils/poissonDisk.hpp"
#include <ctime>
#include <memory>
#include <print>
#include <random>
#include <ranges>

auto SceneBuilder::create_forest_clock_scene(
    // NOLINTNEXTLINE
    const std::shared_ptr<zg::ShaderProgram>& shader_program,
    const std::shared_ptr<zg::ShaderProgram>& tree_program,
    const std::shared_ptr<zg::ShaderProgram>& fixed_shader_program,
    const std::shared_ptr<zg::ShaderProgram>& clock_shader_program
) -> std::unique_ptr<zc::Scene> {
    auto scene = std::make_unique<zc::Scene>();
    scene->observe_program(shader_program);

    scene->add_object(model(ModelId::Plain), shader_program)
        .add_transformation(zg::Translation{{0.f, -2.1f, 0.f}})
        .add_transformation(zg::Scale{100.f});

    auto points
        = zpg::utils::poisson_disk(666, {-30, -30}, glm::vec2{30, 30}, 5.5f);

    auto bush_points
        = zpg::utils::poisson_disk(667, {-30, -30}, glm::vec2{30, 30}, 4.f);

    std::mt19937 random{666};
    std::uniform_real_distribution<float> rotation{0.f, 360.f};
    std::uniform_real_distribution<float> scale{0.8f, 1.2f};

    int clock_count = 0;

    // FOREST
    auto& forest_group = scene->add_group();
    auto& bush_group = scene->add_group();

    std::println("{}", points.size());
    for (glm::vec2 p : std::views::take(points, 76)) {
        auto& tree_group = forest_group.add_group();
        const float tree_scale = scale(random);

        tree_group.add_transformation(zg::Translation{{p.x, -1.f, p.y}});

        tree_group.add_object(model(ModelId::TreeColored), tree_program)
            .add_transformation(zg::Translation{{0, -1, 0}})
            .add_transformation(zg::Rotation{{0.f, 1.f, 0.f}, rotation(random)})
            .add_transformation(
                zg::Scale{
                  {2.3f * tree_scale, 5.f * tree_scale, 2.3f * tree_scale}
                }
            );

        if (clock_count == 1 || clock_count == 4) {
            tree_group.add_object(model(ModelId::Clock), clock_shader_program)
                .add_transformation(zg::Translation({0.f, 2.f, -0.41f}))
                .add_transformation(zg::Rotation{{0.f, 1.f, 0.f}, 0.f})
                .add_transformation(zg::Scale{0.4f});
        }
        clock_count++;
    }

    for (glm::vec2 p : std::views::take(bush_points, 100)) {
        const float bush_scale = scale(random);

        bush_group.add_object(model(ModelId::BushColored), shader_program)
            .add_transformation(zg::Translation{{p.x, -2.f, p.y}})
            .add_transformation(zg::Scale{2.f * bush_scale})
            .add_transformation(zg::Rotation{{0, 1, 0}, rotation(random)});
    }

    scene->add_object(model(ModelId::Slenderman), shader_program)
        .add_transformation(zg::Rotation{{0.f, 1.f, 0.f}, -90.f})
        .add_transformation(zg::Translation{{0.f, -0.15f, 0.f}})
        .add_transformation(zg::Scale{2.2f})
        .add_transformation(zg::Rotation{{0.f, 1.f, 0.f}, -50.f});

    scene->set_update([clock_shader_program](float) -> void {
        const auto now = std::time(nullptr);
        const auto time = *std::localtime(&now);

        clock_shader_program->set_shader_program();

        clock_shader_program->set_uniform(
            clock_shader_program->get_uniform_location("clockTime"),
            static_cast<float>(time.tm_hour % 12) / 12,
            static_cast<float>(time.tm_min) / 60,
            static_cast<float>(time.tm_sec % 60) / 60
        );

        clock_shader_program->unset_shader_program();
    });

    add_signature(scene, fixed_shader_program);

    return scene;
}
