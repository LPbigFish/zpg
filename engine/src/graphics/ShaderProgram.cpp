#include "zpg/graphics/ShaderProgram.hpp"
#include "zpg/core/GlType.hpp"
#include "zpg/graphics/Shader.hpp"
#include <concepts>
#include <stdexcept>
#include <string>
#include <string_view>
#include <type_traits>

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

// NOLINTNEXTLINE
auto ShaderProgram::unset_shader_program() -> void {
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

template<core::GlTypeConstrain T>
auto ShaderProgram::set_uniform(GLint location, T v0) -> void {
    if (location == -1) {
        throw std::runtime_error{
          std::format("Invalid location for uniform in shader program: {}", id)
        };
    }

    // removeneme reference a const reference
    using U = std::remove_cvref_t<T>;

    this->set_shader_program();

    if constexpr (std::same_as<U, GLfloat>) {
        glUniform1f(location, v0);
    } else if constexpr (std::same_as<U, GLuint>) {
        glUniform1ui(location, v0);
    } else if constexpr (std::same_as<U, GLint>) {
        glUniform1i(location, v0);
    }

    this->unset_shader_program();
}

template<core::GlTypeConstrain T>
auto ShaderProgram::set_uniform(GLint location, T v0, T v1) -> void {
    if (location == -1) {
        throw std::runtime_error{
          std::format("Invalid location for uniform in shader program: {}", id)
        };
    }

    // removeneme reference a const reference
    using U = std::remove_cvref_t<T>;

    this->set_shader_program();

    if constexpr (std::same_as<U, GLfloat>) {
        glUniform2f(location, v0, v1);
    } else if constexpr (std::same_as<U, GLuint>) {
        glUniform2ui(location, v0, v1);
    } else if constexpr (std::same_as<U, GLint>) {
        glUniform2i(location, v0, v1);
    }

    this->unset_shader_program();
}

template<core::GlTypeConstrain T>
auto ShaderProgram::set_uniform(GLint location, T v0, T v1, T v2) -> void {
    if (location == -1) {
        throw std::runtime_error{
          std::format("Invalid location for uniform in shader program: {}", id)
        };
    }

    // removeneme reference a const reference
    using U = std::remove_cvref_t<T>;

    this->set_shader_program();

    if constexpr (std::same_as<U, GLfloat>) {
        glUniform3f(location, v0, v1, v2);
    } else if constexpr (std::same_as<U, GLuint>) {
        glUniform3ui(location, v0, v1, v2);
    } else if constexpr (std::same_as<U, GLint>) {
        glUniform3i(location, v0, v1, v2);
    }

    this->unset_shader_program();
}

template<core::GlTypeConstrain T>
auto ShaderProgram::set_uniform(GLint location, T v0, T v1, T v2, T v3)
    -> void {
    if (location == -1) {
        throw std::runtime_error{
          std::format("Invalid location for uniform in shader program: {}", id)
        };
    }

    // removeneme reference a const reference
    using U = std::remove_cvref_t<T>;

    this->set_shader_program();

    if constexpr (std::same_as<U, GLfloat>) {
        glUniform4f(location, v0, v1, v2, v3);
    } else if constexpr (std::same_as<U, GLuint>) {
        glUniform4ui(location, v0, v1, v2, v3);
    } else if constexpr (std::same_as<U, GLint>) {
        glUniform4i(location, v0, v1, v2, v3);
    }

    this->unset_shader_program();
}
} // namespace zpg::graphics
