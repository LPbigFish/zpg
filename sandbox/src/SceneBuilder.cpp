#include "SceneBuilder.hpp"
#include "models/text.h"
#include "zpg/core/Scene.hpp"
#include "zpg/graphics/Rotation.hpp"
#include "zpg/graphics/Scale.hpp"
#include "zpg/graphics/ShaderProgram.hpp"
#include "zpg/graphics/Translation.hpp"
#include <memory>

auto SceneBuilder::add_signature(
    const std::unique_ptr<zc::Scene>& scene,
    const std::shared_ptr<zg::ShaderProgram>& shader_program
) -> void {
    scene->add_object<zg::VertexLayout::PosNormal>(TEXT_POINTS, shader_program)
        .add_transformation(zg::Translation{{0.81f, -0.89f, 0.f}})
        .add_transformation(zg::Rotation{{0.f, 1.f, 0.f}, 20.0f})
        .add_transformation(zg::Scale{0.15f});
}
