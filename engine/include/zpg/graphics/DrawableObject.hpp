#pragma once

#include "zpg/graphics/Model.hpp"
#include "zpg/graphics/SceneNode.hpp"
#include "zpg/graphics/ShaderProgram.hpp"
#include <glm/ext/matrix_float4x4.hpp>
#include <memory>

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
