#pragma once
#include "zpg/core/GlType.hpp"
#include "zpg/graphics/Shader.hpp"
#include <print>
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

    auto set_shader_program() const noexcept -> void;
    auto unset_shader_program() const noexcept -> void;

    [[nodiscard]] auto get_uniform_location(std::string_view path) const
        -> GLint;

    template<core::GlTypeConstrain T>
    auto set_uniform(GLint location, T v0) -> void {
        if (location == -1) {
            std::println(
                stderr, "Invalid location for uniform in shader program: {}", id
            );
            return;
        }

        // removeneme reference a const reference
        using U = std::remove_cvref_t<T>;

        if constexpr (std::same_as<U, GLfloat>) {
            glUniform1f(location, v0);
        } else if constexpr (std::same_as<U, GLuint>) {
            glUniform1ui(location, v0);
        } else if constexpr (std::same_as<U, GLint>) {
            glUniform1i(location, v0);
        }
    }

    template<core::GlTypeConstrain T>
    auto set_uniform(GLint location, T v0, T v1) -> void {
        if (location == -1) {
            std::println(
                stderr, "Invalid location for uniform in shader program: {}", id
            );
            return;
        }

        // removeneme reference a const reference
        using U = std::remove_cvref_t<T>;

        if constexpr (std::same_as<U, GLfloat>) {
            glUniform2f(location, v0, v1);
        } else if constexpr (std::same_as<U, GLuint>) {
            glUniform2ui(location, v0, v1);
        } else if constexpr (std::same_as<U, GLint>) {
            glUniform2i(location, v0, v1);
        }
    }

    template<core::GlTypeConstrain T>
    auto set_uniform(GLint location, T v0, T v1, T v2) -> void {
        if (location == -1) {
            std::println(
                stderr, "Invalid location for uniform in shader program: {}", id
            );
            return;
        }

        // removeneme reference a const reference
        using U = std::remove_cvref_t<T>;

        if constexpr (std::same_as<U, GLfloat>) {
            glUniform3f(location, v0, v1, v2);
        } else if constexpr (std::same_as<U, GLuint>) {
            glUniform3ui(location, v0, v1, v2);
        } else if constexpr (std::same_as<U, GLint>) {
            glUniform3i(location, v0, v1, v2);
        }
    }

    template<core::GlTypeConstrain T>
    auto set_uniform(GLint location, T v0, T v1, T v2, T v3) -> void {
        if (location == -1) {
            std::println(
                stderr, "Invalid location for uniform in shader program: {}", id
            );
            return;
        }

        // removeneme reference a const reference
        using U = std::remove_cvref_t<T>;

        if constexpr (std::same_as<U, GLfloat>) {
            glUniform4f(location, v0, v1, v2, v3);
        } else if constexpr (std::same_as<U, GLuint>) {
            glUniform4ui(location, v0, v1, v2, v3);
        } else if constexpr (std::same_as<U, GLint>) {
            glUniform4i(location, v0, v1, v2, v3);
        }
    }
};

} // namespace zpg::graphics
