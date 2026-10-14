#include "SceneBuilder.hpp"
#include "models/TextModel.hpp"
#include "models/earth.h"
#include "models/moon.h"
#include "models/sun.h"
#include "zpg/graphics/Rotation.hpp"
#include "zpg/graphics/RotationRepeat.hpp"
#include "zpg/graphics/Scale.hpp"
#include "zpg/graphics/Translation.hpp"
#include <memory>

auto SceneBuilder::create_solar_scene(
    const std::shared_ptr<zg::ShaderProgram>& shader_program
) -> std::unique_ptr<zc::Scene> {
    auto scene = std::make_unique<zc::Scene>();

    const auto spin = [](const float degrees_per_second) {
        return zg::RotationRepeat{{0.f, 1.f, 0.f}, degrees_per_second};
    };

    auto& solar_system
        = scene->add_group(zg::RotationRepeat{{0.f, 1.f, 0.f}, 1.0f});

    solar_system.add_group()
        .add_object<zg::VertexLayout::PosColorNormal>(sun, shader_program)
        .add_transformation(zg::Scale{0.3f})
        .add_transformation(spin(15.f));

    auto& earth_group
        = solar_system.add_group(zg::Translation{{0.75f, 0.f, 0.f}});
    earth_group
        .add_object<zg::VertexLayout::PosColorNormal>(earth, shader_program)
        .add_transformation(zg::Scale{0.08f})
        .add_transformation(spin(25.f));

    auto& moon_orbit
        = earth_group.add_group(zg::RotationRepeat{{0.f, 1.f, 0.f}, 6.0f});
    moon_orbit.add_group(zg::Translation{{0.15f, 0.f, 0.f}})
        .add_object<zg::VertexLayout::PosColorNormal>(moon, shader_program)
        .add_transformation(zg::Scale{0.008f})
        .add_transformation(spin(40.f));

    scene->add_object<zg::VertexLayout::PosNormal>(text_model, shader_program)
        .add_transformation(zg::Translation{{0.81f, -0.89f, 0.f}})
        .add_transformation(zg::Rotation{{0.f, 1.f, 0.f}, 20.0f})
        .add_transformation(zg::Scale{0.15f});

    return scene;
}
