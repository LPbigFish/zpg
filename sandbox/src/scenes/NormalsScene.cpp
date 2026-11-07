#include "../SceneBuilder.hpp"
#include "zpg/graphics/Rotation.hpp"
#include "zpg/graphics/Scale.hpp"
#include "zpg/graphics/Translation.hpp"
#include <memory>

auto SceneBuilder::create_normals_scene(
    const std::shared_ptr<zg::ShaderProgram>& wrong_program,
    const std::shared_ptr<zg::ShaderProgram>& right_program,
    const std::shared_ptr<zg::ShaderProgram>& fixed_shader_program
) -> std::unique_ptr<zc::Scene> {
    auto scene = std::make_unique<zc::Scene>();

    auto add_ball
        = [&scene](
              const std::shared_ptr<zg::ShaderProgram>& program, float x
          ) -> void {
        scene->add_object(model(ModelId::SphereNormal), program)
            .add_transformation(zg::Translation{{x, 0.f, 0.f}})
            .add_transformation(zg::Rotation{{0.f, 1.f, 0.f}, 45.f})
            .add_transformation(zg::Scale{{2.f, 0.4f, 1.f}});
    };

    add_ball(wrong_program, -1.6f);
    add_ball(right_program, 1.6f);

    scene->get_camera().set_position({0.f, 0.6f, 4.5f});

    add_signature(scene, fixed_shader_program);

    return scene;
}
