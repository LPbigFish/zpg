#include "zpg/graphics/ShaderProgram.hpp"
#include "zpg/core/GlType.hpp"
#include "zpg/graphics/Shader.hpp"
#include <glm/gtc/type_ptr.hpp>
#include <print>
#include <string>
#include <string_view>
#include <utility>

namespace zpg::graphics {
ShaderProgram::ShaderProgram(
    const VertexShader& vertex, const FragmentShader& fragment
): id{glCreateProgram()} {
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

auto ShaderProgram::on_camera_changed(
    const glm::mat4& view, const glm::mat4& projection
) -> void {
    set_shader_program();

    const auto view_loc = get_uniform_location("viewMatrix");

    const auto projection_loc = get_uniform_location("projectionMatrix");

    if (view_loc >= 0) {
        set_uniform(view_loc, view);
    }

    if (projection_loc >= 0) {
        set_uniform(projection_loc, projection);
    }

    unset_shader_program();
}

auto ShaderProgram::set_uniform(GLint location, const glm::mat4& matrix)
    -> void {
    if (location == -1) {
        std::println(
            stderr, "Invalid location for uniform in shader program: {}", id
        );
        return;
    }

    glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
}
} // namespace zpg::graphics
