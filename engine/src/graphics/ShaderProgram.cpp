#include "zpg/graphics/ShaderProgram.hpp"
#include "zpg/core/GlType.hpp"
#include "zpg/graphics/Shader.hpp"
#include <string>
#include <string_view>

namespace zpg::graphics {
ShaderProgram::ShaderProgram(VertexShader& vertex, FragmentShader& fragment):
    id{glCreateProgram()} {
    glAttachShader(id, vertex.get());
    glAttachShader(id, fragment.get());
    glLinkProgram(id);
}

auto ShaderProgram::set_shader_program() const noexcept -> void {
    glUseProgram(id);
}

// NOLINTNEXTLINE
auto ShaderProgram::unset_shader_program() const noexcept -> void {
    glUseProgram(0);
}

ShaderProgram::~ShaderProgram() {
    glDeleteProgram(id);
}

ShaderProgram::ShaderProgram(ShaderProgram&& other) noexcept:
    id{std::exchange(other.id, 0)} {}

auto ShaderProgram::get_uniform_location(std::string_view path) const -> GLint {
    std::string temp{path};
    return glGetUniformLocation(this->id, temp.c_str());
}
} // namespace zpg::graphics
