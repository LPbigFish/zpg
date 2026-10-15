#include "SceneBuilder.hpp"
#include "models/SM_Model.hpp"
#include "zpg/graphics/Rotation.hpp"
#include "zpg/graphics/Scale.hpp"
#include "zpg/graphics/Translation.hpp"
#include <memory>

auto SceneBuilder::create_slenderman_scene(
    const std::shared_ptr<zg::ShaderProgram>& shader_program
) -> std::unique_ptr<zc::Scene> {
    auto scene = std::make_unique<zc::Scene>();

    scene
        ->add_object<zg::VertexLayout::PosColorNormal>(SM_Model, shader_program)
        .add_transformation(zg::Translation{{-0.5f, -0.1f, 0.f}})
        .add_transformation(zg::Rotation{{0.f, 1.f, 0.f}, -90.f})
        .add_transformation(zg::Scale{0.7f})
        .add_transformation(zg::Rotation{{0.f, 1.f, 0.f}, -30.f});

    add_signature(scene, shader_program);

    return scene;
}
