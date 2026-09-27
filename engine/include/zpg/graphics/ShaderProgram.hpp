#pragma once
#include "zpg/graphics/Shader.hpp"

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
};

} // namespace zpg::graphics
