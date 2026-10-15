#include "SceneBuilder.hpp"
#include "models/clock.h"
#include <memory>

auto SceneBuilder::create_forest_clock_scene(
    const std::shared_ptr<zg::ShaderProgram>& shader_program
) -> std::unique_ptr<zc::Scene> {
    auto scene = std::make_unique<zc::Scene>();
    scene->add_object<zg::VertexLayout::PosNormalColor>(
        clockModel, shader_program
    );

    add_signature(scene, shader_program);

    return scene;
}
