#include "SceneBuilder.hpp"
#include "zpg/graphics/RotationRepeat.hpp"
#include "zpg/graphics/Scale.hpp"
#include "zpg/graphics/Translation.hpp"
#include <memory>

auto SceneBuilder::create_solar_scene(
    const std::shared_ptr<zg::ShaderProgram>& ambient_program,
    const std::shared_ptr<zg::ShaderProgram>& phong_program,
    const std::shared_ptr<zg::ShaderProgram>& blinn_program,
    const std::shared_ptr<zg::ShaderProgram>& fixed_shader_program
) -> std::unique_ptr<zc::Scene> {
    auto scene = std::make_unique<zc::Scene>();

    scene->set_update(
        [ambient_program, phong_program, blinn_program](float) -> void {
            for (const auto& program :
                 {ambient_program, phong_program, blinn_program}) {
                program->set_shader_program();

                auto lp = program->get_uniform_location("lightPosition");

                if (lp != -1) {
                    program->set_uniform(lp, 0.f, 0.f, 0.f);
                }

                auto color = program->get_uniform_location("color");

                if (color != -1) {
                    program->set_uniform(color, -1.f, -1.f, -1.f);
                }

                program->unset_shader_program();
            }

            {
                ambient_program->set_shader_program();
                ambient_program->set_uniform(
                    ambient_program->get_uniform_location("ambient"),
                    1.5f,
                    1.5f,
                    1.5f
                );
                ambient_program->unset_shader_program();
            }
        }
    );

    const auto spin = [](const float degrees_per_second) -> zg::RotationRepeat {
        return zg::RotationRepeat{{0.f, 1.f, 0.1f}, degrees_per_second};
    };

    auto& solar_system = scene->add_group();

    solar_system.add_group()
        .add_object(model(ModelId::Sun), ambient_program)
        .add_transformation(zg::Scale{0.3f})
        .add_transformation(spin(15.f));

    auto& earth_group
        = solar_system.add_group(zg::Translation{{0.75f, 0.f, 0.f}});
    earth_group.add_object(model(ModelId::Earth), phong_program)
        .add_transformation(zg::Scale{0.08f})
        .add_transformation(spin(25.f));

    auto& moon_orbit
        = earth_group.add_group(zg::RotationRepeat{{0.f, 1.f, 0.f}, 6.0f});
    moon_orbit.add_group(zg::Translation{{0.4f, 0.f, 0.f}})
        .add_object(model(ModelId::Moon), phong_program)
        .add_transformation(zg::Scale{0.02f})
        .add_transformation(spin(40.f));

    auto& satellite_orbit = earth_group.add_group(
        zg::RotationRepeat{{0.f, 0.9063f, 0.4226f}, 10.f}
    );
    satellite_orbit.add_group(zg::Translation{{0.23f, 0.f, 0.f}})
        .add_object(model(ModelId::Satellite), blinn_program)
        .add_transformation(zg::Scale{0.05f})
        .add_transformation(spin(4.f));

    add_signature(scene, fixed_shader_program);

    return scene;
}
