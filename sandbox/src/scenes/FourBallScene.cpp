#include "../SceneBuilder.hpp"
#include "zpg/core/Scene.hpp"
#include "zpg/graphics/Scale.hpp"
#include "zpg/graphics/Translation.hpp"
#include "zpg/graphics/VertexArray.hpp"
#include <array>
#include <memory>

auto SceneBuilder::create_four_ball_scene(
    const std::shared_ptr<zg::ShaderProgram>& ambient_program,
    const std::shared_ptr<zg::ShaderProgram>& lambert_program,
    const std::shared_ptr<zg::ShaderProgram>& phong_program,
    const std::shared_ptr<zg::ShaderProgram>& blinn_program
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

    for (const auto& program :
         {lambert_program, phong_program, blinn_program}) {
        program->set_shader_program();
        program->set_uniform(
            program->get_uniform_location("lightPosition"), 0.f, 0.f, 0.f
        );
        program->unset_shader_program();
    }

    for (std::size_t i = 0; i < POSITIONS.size(); ++i) {
        const auto& [x, z] = POSITIONS.at(i);
        scene->add_object<zg::VertexLayout::PosNormal>(sphere, programs.at(i))
            .add_transformation(zg::Translation{{1.5f * x, 0.f, 1.5f * z}})
            .add_transformation(zg::Scale{0.65f});
    }

    scene->get_camera().set_position({0.f, 6.f, 0.f});
    scene->get_camera().look(0.f, -890.f);

    return scene;
}
