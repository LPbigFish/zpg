#pragma once
#include "zpg/core/GlType.hpp"
#include "zpg/graphics/Shader.hpp"
#include <string_view>

namespace zpg::graphics {

class ShaderProgram {
    GLuint id;

  public:
    ShaderProgram() = delete;
    ~ShaderProgram();
    ShaderProgram(VertexShader& vertex, FragmentShader& fragment);

    ShaderProgram(ShaderProgram&& other) noexcept;

    ShaderProgram(const ShaderProgram&) = delete;
    auto operator=(const ShaderProgram&) -> ShaderProgram& = delete;
    auto operator=(ShaderProgram&&) -> ShaderProgram& = delete;

    auto set_shader_program() const -> void;
    auto unset_shader_program() -> void;

    [[nodiscard]] auto get_uniform_location(std::string_view path) const
        -> GLint;

    template<core::GlTypeConstrain T>
    auto set_uniform(GLint location, T v0) -> void;

    template<core::GlTypeConstrain T>
    auto set_uniform(GLint location, T v0, T v1) -> void;

    template<core::GlTypeConstrain T>
    auto set_uniform(GLint location, T v0, T v1, T v2) -> void;

    template<core::GlTypeConstrain T>
    auto set_uniform(GLint location, T v0, T v1, T v2, T v3) -> void;
};

} // namespace zpg::graphics
