#include "SceneBuilder.hpp"
#include "models/TextModel.hpp"
#include "zpg/graphics/Rotation.hpp"
#include "zpg/graphics/Scale.hpp"
#include "zpg/graphics/Translation.hpp"
#include <array>
#include <memory>

auto SceneBuilder::create_triangles_scene(
    const std::shared_ptr<zg::ShaderProgram>& shader_program
) -> std::unique_ptr<zc::Scene> {
    auto scene = std::make_unique<zc::Scene>();

    constexpr std::array<float, 18> POINTS_TRIANGLE_A{
      -0.5f,
      -0.5f,
      0.0f,
      1.0f,
      0.0f,
      0.0f,
      0.5f,
      -0.5f,
      0.0f,
      0.0f,
      1.0f,
      0.0f,
      0.0f,
      0.5f,
      0.0f,
      0.0f,
      0.0f,
      1.0f
    };
    constexpr std::array<float, 18> POINTS_TRIANGLE_B{
      -0.5f,
      -0.5f,
      0.0f,
      0.0f,
      1.0f,
      1.0f,
      0.5f,
      -0.5f,
      0.0f,
      1.0f,
      0.0f,
      1.0f,
      0.0f,
      0.5f,
      0.0f,
      1.0f,
      1.0f,
      0.0f
    };

    // A: T applied first, then R (rotate around world origin).
    scene
        ->add_object<zg::VertexLayout::PosColor>(
            POINTS_TRIANGLE_A, shader_program
        )
        .add_transformation(zg::Scale{0.8f})
        .add_transformation(zg::Translation{{0.45f, -0.45f, 0.f}})
        .add_transformation(zg::Rotation{{0.f, 0.f, 1.f}, 90.0f});

    // B: R applied first (spin in place), then T.
    scene
        ->add_object<zg::VertexLayout::PosColor>(
            POINTS_TRIANGLE_B, shader_program
        )
        .add_transformation(zg::Scale{0.8f})
        .add_transformation(zg::Rotation{{0.f, 0.f, 1.f}, 90.0f})
        .add_transformation(zg::Translation{{0.45f, -0.45f, 0.f}});

    scene->add_object<zg::VertexLayout::PosNormal>(text_model, shader_program)
        .add_transformation(zg::Translation{{0.81f, -0.89f, 0.f}})
        .add_transformation(zg::Rotation{{0.f, 1.f, 0.f}, 20.0f})
        .add_transformation(zg::Scale{0.15f});

    return scene;
}
