#include "zpg/graphics/ShaderProgram.hpp"
#include "zpg/graphics/Shader.hpp"

namespace zpg::graphics {
ShaderProgram::ShaderProgram(VertexShader& vertex, FragmentShader& fragment):
    id{glCreateProgram()} {
    glAttachShader(id, vertex.get());
    glAttachShader(id, fragment.get());
    glLinkProgram(id);
}

auto ShaderProgram::set_shader_program() const -> void {
    glUseProgram(id);
}

ShaderProgram::~ShaderProgram() {
    glDeleteProgram(id);
}

ShaderProgram::ShaderProgram(ShaderProgram&& other) noexcept:
    id{std::exchange(other.id, 0)} {}
} // namespace zpg::graphics
