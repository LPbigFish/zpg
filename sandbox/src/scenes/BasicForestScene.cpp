#include "SceneBuilder.hpp"
#include "models/TextModel.hpp"
#include "models/bushes.h"
#include "models/sphere.h"
#include "models/tree.h"
#include "zpg/graphics/Rotation.hpp"
#include "zpg/graphics/Scale.hpp"
#include "zpg/graphics/Translation.hpp"
#include <cstdlib>
#include <memory>
#include <span>
#include <vector>

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
} // namespace

auto SceneBuilder::create_basic_forest_scene(
    const std::shared_ptr<zg::ShaderProgram>& shader_program,
    const std::shared_ptr<zg::ShaderProgram>& shader_program_2
) -> std::unique_ptr<zc::Scene> {
    auto scene = std::make_unique<zc::Scene>();

    scene->add_object<zg::VertexLayout::PosNormal>(text_model, shader_program)
        .add_transformation(zg::Translation{{0.81f, -0.89f, 0.f}})
        .add_transformation(zg::Rotation{{0.f, 1.f, 0.f}, 20.0f})
        .add_transformation(zg::Scale{0.15f});

    scene->add_object<zg::VertexLayout::PosColor>(sphere, shader_program_2)
        .add_transformation(zg::Translation{{0.f, 0.62f, 0.95f}})
        .add_transformation(zg::Scale{0.22f});

    shader_program_2->set_shader_program();
    const auto location
        = shader_program_2->get_uniform_location("fragmentColor");
    shader_program_2->set_uniform(location, 1.f, 1.f, 0.f);
    shader_program_2->unset_shader_program();

    srand(777);
    constexpr auto RANDOM_BETWEEN = [](const float min, const float max) {
        return min
             + (static_cast<float>(rand())
                / (static_cast<float>(RAND_MAX) / (max - min)));
    };
    const auto tree_model = make_colored_tree();

    for (std::size_t i = 0; i < 12; ++i) {
        const auto base_position = glm::vec3{
          RANDOM_BETWEEN(-0.95f, 0.95f),
          RANDOM_BETWEEN(-0.85f, -0.02f),
          RANDOM_BETWEEN(0.05f, 0.75f)
        };

        scene
            ->add_object<zg::VertexLayout::PosColorNormal>(
                tree_model, shader_program
            )
            .add_transformation(zg::Translation{base_position})
            .add_transformation(
                zg::Rotation{{0.f, 1.f, 0.f}, RANDOM_BETWEEN(0.f, 360.f)}
            )
            .add_transformation(zg::Scale{0.08f * RANDOM_BETWEEN(0.8f, 1.2f)});

        scene->add_object<zg::VertexLayout::PosColor>(bushes, shader_program)
            .add_transformation(zg::Translation{base_position})
            .add_transformation(
                zg::Translation{
                  {RANDOM_BETWEEN(-0.08f, 0.08f),
                   0.f,
                   RANDOM_BETWEEN(-0.08f, 0.08f)}
                }
            )
            .add_transformation(
                zg::Rotation{{0.f, 1.f, 0.f}, RANDOM_BETWEEN(0.f, 360.f)}
            )
            .add_transformation(zg::Scale{0.25f * RANDOM_BETWEEN(0.8f, 1.2f)});
    }

    return scene;
}
