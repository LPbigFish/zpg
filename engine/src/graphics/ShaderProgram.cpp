#include "zpg/graphics/ShaderProgram.hpp"
#include "zpg/core/GlType.hpp"
#include "zpg/graphics/Shader.hpp"
#include <string_view>

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

auto ShaderProgram::unset_shader_program() -> void {
    glUseProgram(0);
}

ShaderProgram::~ShaderProgram() {
    glDeleteProgram(id);
}

ShaderProgram::ShaderProgram(ShaderProgram&& other) noexcept:
    id{std::exchange(other.id, 0)} {}

auto ShaderProgram::get_uniform_location(std::string_view path) const -> GLint {
    return glGetUniformLocation(this->id, path.cbegin());
}
} // namespace zpg::graphics
