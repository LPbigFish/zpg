#pragma once

#include "zpg/graphics/Model.hpp"
#include "zpg/graphics/SceneNode.hpp"
#include "zpg/graphics/ShaderProgram.hpp"
#include "zpg/graphics/TransformationComposite.hpp"
#include <concepts>
#include <glm/ext/matrix_float4x4.hpp>
#include <glm/ext/vector_float3.hpp>
#include <glm/trigonometric.hpp>
#include <memory>
#include <numbers>
#include <utility>

constexpr auto RADIANTS = []() -> std::array<float, 360> {
    std::array<float, 360> res{};
    for (std::size_t i = 0; i < 360; i++) {
        res.at(i) = static_cast<float>(i) * (std::numbers::pi_v<float> / 180);
    }
    return res;
}();

namespace zpg::graphics {
class DrawableObject final: public SceneNode {
    Model model;
    std::shared_ptr<ShaderProgram> shader_program;
    std::shared_ptr<TransformationComposite> _transformation
        = std::make_shared<TransformationComposite>();

    auto render_in_world(const glm::mat4& mat) -> void override;

  public:
    DrawableObject() = delete;

    explicit DrawableObject(
        Model model, std::shared_ptr<ShaderProgram> shader_program
    );

    auto
    add_transformation(std::unique_ptr<TransformationComponent> transformation)
        -> DrawableObject&;

    template<std::derived_from<TransformationComponent> T>
    auto add_transformation(T transformation) -> DrawableObject& {
        return add_transformation(
            std::make_unique<T>(std::move(transformation))
        );
    }

    auto add_transformation_front(
        std::unique_ptr<TransformationComponent> transformation
    ) -> DrawableObject&;

    template<std::derived_from<TransformationComponent> T>
    auto add_transformation_front(T transformation) -> DrawableObject& {
        return add_transformation_front(
            std::make_unique<T>(std::move(transformation))
        );
    }

    [[nodiscard]] auto get_transformation_composite() const
        -> const std::shared_ptr<TransformationComposite>&;

    auto draw() noexcept -> void;

    [[nodiscard]] auto get_program() const -> const ShaderProgram&;

    DrawableObject(DrawableObject&& other) noexcept;

    ~DrawableObject() = default;
    auto operator=(DrawableObject&& other) noexcept -> DrawableObject& = delete;
    DrawableObject(const DrawableObject& other) = delete;
    auto operator=(const DrawableObject& other) -> DrawableObject& = delete;
};
} // namespace zpg::graphics
