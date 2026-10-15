#include "SceneBuilder.hpp"
#include "models/text.h"
#include "zpg/graphics/Rotation.hpp"
#include "zpg/graphics/Scale.hpp"
#include <memory>

auto SceneBuilder::create_login_scene(
    const std::shared_ptr<zg::ShaderProgram>& shader_program
) -> std::unique_ptr<zc::Scene> {
    auto scene = std::make_unique<zc::Scene>();

    scene->add_object<zg::VertexLayout::PosNormal>(TEXT_POINTS, shader_program)
        .add_transformation(zg::Rotation{{0.f, 1.f, 0.f}, 32.0f})
        .add_transformation(zg::Scale{0.5f});

    add_signature(scene, shader_program);

    return scene;
}
