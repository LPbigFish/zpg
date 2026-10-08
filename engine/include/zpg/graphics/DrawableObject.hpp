#pragma once

#include "zpg/graphics/Model.hpp"
#include "zpg/graphics/SceneNode.hpp"
#include "zpg/graphics/ShaderProgram.hpp"
#include <glm/ext/matrix_float4x4.hpp>
#include <glm/ext/vector_float3.hpp>
#include <glm/trigonometric.hpp>
#include <numbers>

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

    auto render_in_world(const glm::mat4& mat) -> void override;

  public:
    DrawableObject() = delete;

    explicit DrawableObject(
        Model model, std::shared_ptr<ShaderProgram> shader_program
    );

    auto draw() noexcept -> void;

    [[nodiscard]] auto get_program() const -> const ShaderProgram&;

    DrawableObject(DrawableObject&& other) noexcept;

    ~DrawableObject() = default;
    auto operator=(DrawableObject&& other) noexcept -> DrawableObject& = delete;
    DrawableObject(const DrawableObject& other) = delete;
    auto operator=(const DrawableObject& other) -> DrawableObject& = delete;
};
} // namespace zpg::graphics
