#include "SceneBuilder.hpp"
#include "models/SM_Model.hpp"
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

namespace {
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

auto SceneBuilder::create_forest_clock_scene(
    // NOLINTNEXTLINE
    const std::shared_ptr<zg::ShaderProgram>& shader_program,
    const std::shared_ptr<zg::ShaderProgram>& tree_program,
    const std::shared_ptr<zg::ShaderProgram>& fixed_shader_program,
    const std::shared_ptr<zg::ShaderProgram>& clock_shader_program
) -> std::unique_ptr<zc::Scene> {
    auto scene = std::make_unique<zc::Scene>();
    scene->observe_program(shader_program);

    scene->add_object<zg::VertexLayout::PosNormal>(PLAIN, shader_program)
        .add_transformation(zg::Translation{{0.f, -2.1f, 0.f}})
        .add_transformation(zg::Scale{100.f});

    auto points
        = zpg::utils::poisson_disk(666, {-30, -30}, glm::vec2{30, 30}, 5.5f);

    auto bush_points
        = zpg::utils::poisson_disk(667, {-30, -30}, glm::vec2{30, 30}, 4.f);

    const auto tree_model = make_colored_tree();
    const auto bush_model = make_colored_bush();

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

        tree_group
            .add_object<zg::VertexLayout::PosColorNormal>(
                tree_model, tree_program
            )
            .add_transformation(zg::Translation{{0, -1, 0}})
            .add_transformation(zg::Rotation{{0.f, 1.f, 0.f}, rotation(random)})
            .add_transformation(
                zg::Scale{
                  {2.3f * tree_scale, 5.f * tree_scale, 2.3f * tree_scale}
                }
            );

        if (clock_count == 1 || clock_count == 4) {
            tree_group
                .add_object<zg::VertexLayout::PosNormalColor>(
                    clockModel, clock_shader_program
                )
                .add_transformation(zg::Translation({0.f, 2.f, -0.41f}))
                .add_transformation(zg::Rotation{{0.f, 1.f, 0.f}, 0.f})
                .add_transformation(zg::Scale{0.4f});
        }
        clock_count++;
    }

    for (glm::vec2 p : std::views::take(bush_points, 100)) {
        const float bush_scale = scale(random);

        bush_group
            .add_object<zg::VertexLayout::PosColorNormal>(
                bush_model, shader_program
            )
            .add_transformation(zg::Translation{{p.x, -2.f, p.y}})
            .add_transformation(zg::Scale{2.f * bush_scale})
            .add_transformation(zg::Rotation{{0, 1, 0}, rotation(random)});
    }

    scene
        ->add_object<zg::VertexLayout::PosColorNormal>(SM_Model, shader_program)
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
