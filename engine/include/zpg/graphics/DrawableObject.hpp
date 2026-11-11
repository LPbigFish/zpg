#pragma once

#include "zpg/graphics/Model.hpp"
#include "zpg/graphics/SceneNode.hpp"
#include "zpg/graphics/ShaderProgram.hpp"
#include "zpg/graphics/Texture.hpp"
#include <glad/gl.h>
#include <glm/ext/matrix_float4x4.hpp>
#include <memory>
#include <optional>

namespace zpg::graphics {
class DrawableObject final: public SceneNode {
    inline static GLint next_id{1};
    GLint id{next_id++};

    std::shared_ptr<const Model> model;
    std::shared_ptr<ShaderProgram> shader_program;
    std::optional<std::shared_ptr<Texture>> texture;

    auto render_in_world(const glm::mat4& mat) -> void override;

  public:
    DrawableObject() = delete;

    explicit DrawableObject(
        std::shared_ptr<const Model> model,
        std::shared_ptr<ShaderProgram> shader_program,
        std::optional<std::shared_ptr<Texture>> texture = std::nullopt
    );

    auto draw() noexcept -> void;

    [[nodiscard]] auto get_id() const noexcept -> GLint;
    [[nodiscard]] auto get_program() const -> const ShaderProgram&;

    DrawableObject(DrawableObject&& other) noexcept;

    ~DrawableObject() = default;
    auto operator=(DrawableObject&& other) noexcept -> DrawableObject& = delete;
    DrawableObject(const DrawableObject& other) = delete;
    auto operator=(const DrawableObject& other) -> DrawableObject& = delete;
};
} // namespace zpg::graphics
