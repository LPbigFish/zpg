#include "SceneBuilder.hpp"
#include "models/sphere.h"
#include "models/text.h"
#include "zpg/graphics/Rotation.hpp"
#include "zpg/graphics/Scale.hpp"
#include "zpg/graphics/Translation.hpp"
#include <memory>

auto SceneBuilder::create_ball_scene(
    const std::shared_ptr<zg::ShaderProgram>& shader_program
) -> std::unique_ptr<zc::Scene> {
    auto scene = std::make_unique<zc::Scene>();

    scene->add_object<zg::VertexLayout::PosNormal>(TEXT_POINTS, shader_program)
        .add_transformation(zg::Translation{{0.81f, -0.89f, 0.f}})
        .add_transformation(zg::Rotation{{0.f, 1.f, 0.f}, 20.0f})
        .add_transformation(zg::Scale{0.15f});

    scene->add_object<zg::VertexLayout::PosColor>(sphere, shader_program)
        .add_transformation(zg::Translation{{0.f, 0.1f, 0.f}})
        .add_transformation(zg::Rotation{{0.f, 1.f, 0.f}, 45.0f})
        .add_transformation(zg::Scale{0.2f});

    return scene;
}
