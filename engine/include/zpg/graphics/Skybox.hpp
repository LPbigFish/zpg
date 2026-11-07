#pragma once

#include "zpg/core/Camera.hpp"
#include "zpg/graphics/Model.hpp"
#include "zpg/graphics/ShaderProgram.hpp"
#include "zpg/graphics/Texture.hpp"
#include <glm/ext/matrix_float4x4.hpp>
#include <memory>
#include <span>
#include <string>

namespace zpg::graphics {
class Skybox final {
    std::shared_ptr<const Model> model;
    std::shared_ptr<ShaderProgram> program;
    Texture texture;

    Skybox(
        std::shared_ptr<const Model> model,
        std::shared_ptr<ShaderProgram> shader_program,
        Texture&& texture
    );

  public:
    Skybox() = delete;
    Skybox(Skybox&& other) noexcept = default;
    ~Skybox() = default;

    Skybox(const Skybox&) = delete;
    auto operator=(const Skybox&) -> Skybox& = delete;
    auto operator=(Skybox&&) -> Skybox& = delete;

    static auto create(
        const std::shared_ptr<ShaderProgram>& shader_program,
        std::shared_ptr<const Model> model,
        std::span<const std::string, 6> face_paths
    ) -> std::expected<Skybox, std::string>;

    auto render(const core::Camera& camera) const -> void;
};
} // namespace zpg::graphics
