#include "SceneBuilder.hpp"
#include "zpg/graphics/Rotation.hpp"
#include "zpg/graphics/Scale.hpp"
#include "zpg/graphics/Translation.hpp"
#include <cstdlib>
#include <memory>

auto SceneBuilder::create_basic_forest_scene(
    // NOLINTNEXTLINE
    const std::shared_ptr<zg::ShaderProgram>& shader_program,
    const std::shared_ptr<zg::ShaderProgram>& shader_program_2
) -> std::unique_ptr<zc::Scene> {
    auto scene = std::make_unique<zc::Scene>();

    scene->add_object(model(ModelId::SphereColor), shader_program_2)
        .add_transformation(zg::Translation{{0.f, 0.62f, 0.95f}})
        .add_transformation(zg::Scale{0.22f});

    shader_program_2->set_shader_program();
    const auto location
        = shader_program_2->get_uniform_location("fragmentColor");
    shader_program_2->set_uniform(location, 1.f, 1.f, 0.f);
    shader_program_2->unset_shader_program();

    srand(777);
    constexpr auto RANDOM_BETWEEN
        = [](const float min, const float max) -> float {
        return min
             + (static_cast<float>(rand())
                / (static_cast<float>(RAND_MAX) / (max - min)));
    };
    int clock_count = 0;

    for (std::size_t i = 0; i < 12; ++i) {
        const auto base_position = glm::vec3{
          RANDOM_BETWEEN(-0.95f, 0.95f),
          RANDOM_BETWEEN(-0.85f, -0.02f),
          RANDOM_BETWEEN(0.05f, 0.75f)
        };

        scene->add_object(model(ModelId::TreeColored), shader_program)
            .add_transformation(zg::Translation{base_position})
            .add_transformation(
                zg::Rotation{{0.f, 1.f, 0.f}, RANDOM_BETWEEN(0.f, 360.f)}
            )
            .add_transformation(zg::Scale{0.08f * RANDOM_BETWEEN(0.8f, 1.2f)});

        clock_count++;
        if (clock_count == 1 || clock_count == 4) {
            scene->add_object(model(ModelId::BushColor), shader_program)
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
                .add_transformation(
                    zg::Scale{0.25f * RANDOM_BETWEEN(0.8f, 1.2f)}
                );
        }
    }

    add_signature(scene, shader_program);

    return scene;
}
