#include "../SceneBuilder.hpp"
#include "zpg/core/Scene.hpp"
#include "zpg/graphics/Scale.hpp"
#include "zpg/graphics/Translation.hpp"
#include "zpg/graphics/VertexArray.hpp"
#include <array>
#include <glm/ext/vector_float3.hpp>
#include <memory>

auto SceneBuilder::create_four_ball_scene(
    const std::shared_ptr<zg::ShaderProgram>& ambient_program,
    const std::shared_ptr<zg::ShaderProgram>& lambert_program,
    const std::shared_ptr<zg::ShaderProgram>& phong_program,
    const std::shared_ptr<zg::ShaderProgram>& blinn_program,
    const std::shared_ptr<zg::ShaderProgram>& fixed_shader_program
) -> std::unique_ptr<zc::Scene> {
    auto scene = std::make_unique<zc::Scene>();

    constexpr std::array<std::array<float, 2>, 4> POSITIONS{{
      {0.f, 1.f},
      {1.f, 0.f},
      {0.f, -1.f},
      {-1.f, 0.f},
    }};
    const std::array<std::shared_ptr<zg::ShaderProgram>, 4> programs{
      ambient_program, lambert_program, phong_program, blinn_program
    };

    scene->set_update([programs](float) -> void {
        for (const auto& program : programs) {
            program->set_shader_program();

            auto lp = program->get_uniform_location("lightPosition");

            if (lp != -1) {
                program->set_uniform(lp, 0.f, 0.f, 0.f);
            }

            auto color = program->get_uniform_location("color");

            if (color != -1) {
                program->set_uniform(color, 0.385f, 0.647f, 0.812f);
            }

            auto ambient = program->get_uniform_location("ambient");

            if (ambient != -1) {
                program->set_uniform(ambient, 0.1f, 0.1f, 0.1f);
            }

            program->unset_shader_program();
        }
    });

    for (std::size_t i = 0; i < POSITIONS.size(); ++i) {
        const auto& [x, z] = POSITIONS.at(i);
        scene->add_object(model(ModelId::SphereNormal), programs.at(i))
            .add_transformation(zg::Translation{{1.5f * x, 0.f, 1.5f * z}})
            .add_transformation(zg::Scale{0.65f});
    }

    scene->get_camera().set_position({0.f, 6.f, 0.f});
    scene->get_camera().look(0.f, -890.f);

    add_signature(scene, fixed_shader_program);

    return scene;
}
