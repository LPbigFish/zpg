#include "../SceneBuilder.hpp"
#include "zpg/core/Scene.hpp"
#include "zpg/graphics/Scale.hpp"
#include "zpg/graphics/Translation.hpp"
#include "zpg/graphics/VertexArray.hpp"
#include <array>
#include <memory>

auto SceneBuilder::create_four_ball_scene(
    const std::shared_ptr<zg::ShaderProgram>& phong_program
) -> std::unique_ptr<zc::Scene> {
    auto scene = std::make_unique<zc::Scene>();

    auto& ball_group = scene->add_group();

    constexpr std::array<std::array<float, 2>, 4> POSITIONS{{
      {0.f, 1.f},
      {1.f, 0.f},
      {0.f, -1.f},
      {-1.f, 0.f},
    }};

    ball_group.add_transformation(zg::Translation{{0.f, 2.f, 0.f}});

    for (const auto& [horizontal_pos, vert_pos] : POSITIONS) {
        ball_group
            .add_object<zg::VertexLayout::PosNormal>(sphere, phong_program)
            .add_transformation(
                zg::Translation{{1.3f * horizontal_pos, 1.3f, 1.3f * vert_pos}}
            )
            .add_transformation(zg::Scale{1.f});
    }

    return scene;
}
