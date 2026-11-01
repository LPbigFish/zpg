#include "../SceneBuilder.hpp"
#include "zpg/graphics/Scale.hpp"
#include "zpg/graphics/Translation.hpp"
#include <memory>

// Task 2a/2b: shininess = 1, light behind the balls so their back side faces
// the camera. Left: specular without the back-side clamp -> highlight bleeds
// through. Right: phong.frag clips specular when dot(N, L) <= 0.
auto SceneBuilder::create_specular_scene(
    const std::shared_ptr<zg::ShaderProgram>& wrong_program,
    const std::shared_ptr<zg::ShaderProgram>& right_program,
    const std::shared_ptr<zg::ShaderProgram>& fixed_shader_program
) -> std::unique_ptr<zc::Scene> {
    auto scene = std::make_unique<zc::Scene>();

    auto add_ball
        = [&scene](
              const std::shared_ptr<zg::ShaderProgram>& program, float x
          ) -> void {
        scene->add_object<zg::VertexLayout::PosNormal>(sphere, program)
            .add_transformation(zg::Translation{{x, 0.f, 0.f}});
    };

    add_ball(wrong_program, -1.6f);
    add_ball(right_program, 1.6f);

    scene->get_camera().set_position({0.f, 0.f, 5.f});

    add_signature(scene, fixed_shader_program);

    return scene;
}
