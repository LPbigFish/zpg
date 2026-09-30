#pragma once
#include "zpg/core/GlType.hpp"
#include "zpg/graphics/Shader.hpp"
#include <string_view>

namespace zpg::graphics {

class ShaderProgram {
    GLuint id;

    [[nodiscard]] auto get_uniform_location(std::string_view path) const
        -> GLint;

  public:
    ShaderProgram() = delete;
    ~ShaderProgram();
    ShaderProgram(VertexShader& vertex, FragmentShader& fragment);

    ShaderProgram(ShaderProgram&& other) noexcept;

    ShaderProgram(const ShaderProgram&) = delete;
    auto operator=(const ShaderProgram&) -> ShaderProgram& = delete;
    auto operator=(ShaderProgram&&) -> ShaderProgram& = delete;

    auto set_shader_program() const -> void;
    static auto unset_shader_program() -> void;

    auto set_uniform_data(std::string_view path, float x) -> void;

    auto set_uniform_data(std::string_view path, double x) -> void;

    auto set_uniform_data(std::string_view path, double x, double y, double z)
        -> void;
    auto set_uniform_data(std::string_view path, float x, float y, float z)
        -> void;
};

} // namespace zpg::graphics
