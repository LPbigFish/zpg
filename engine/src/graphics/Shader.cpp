#include "zpg/graphics/Shader.hpp"
#include <format>
#include <fstream>
#include <iterator>
#include <utility>

namespace zpg::graphics {

template<ShaderType Type> auto Shader<Type>::gl_type() noexcept -> GLenum {
    if constexpr (Type == ShaderType::VERTEX) {
        return GL_VERTEX_SHADER;
    } else {
        return GL_FRAGMENT_SHADER;
    }
}

template<ShaderType Type>
auto Shader<Type>::shader_name() noexcept -> std::string_view {
    if constexpr (Type == ShaderType::VERTEX) {
        return "Vertex";
    } else {
        return "Fragment";
    }
}

template<ShaderType Type>
auto Shader<Type>::create(const fs::path& path)
    -> std::expected<Shader, std::string> {
    if (!fs::exists(path)) {
        return std::unexpected{
          std::format("Shader not found on path: {}", path.string())
        };
    }

    std::ifstream file{path};
    std::string shader_code{
      std::istreambuf_iterator<char>{file}, std::istreambuf_iterator<char>{}
    };
    const auto* c_sc = shader_code.c_str();
    auto id = glCreateShader(gl_type());

    if (id == 0) {
        return std::unexpected{
          std::format("Failed to create {} shader", shader_name())
        };
    }

    glShaderSource(id, 1, &c_sc, nullptr);
    glCompileShader(id);

    GLint success{};
    glGetShaderiv(id, GL_COMPILE_STATUS, &success);
    if (!success) {
        GLint max_length{};
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &max_length);

        std::string info_log(static_cast<std::size_t>(max_length), '\0');
        GLsizei length{};
        glGetShaderInfoLog(id, max_length, &length, info_log.data());
        info_log.resize(static_cast<std::size_t>(length));
        glDeleteShader(id);

        return std::unexpected{std::format(
            "Failed to compile {} shader: \n    {}", shader_name(), info_log
        )};
    }

    return Shader{id};
}

template<ShaderType Type>
Shader<Type>::Shader(Shader&& other) noexcept: id{std::exchange(other.id, 0)} {}

template<ShaderType Type> Shader<Type>::~Shader() {
    glDeleteShader(id);
}

template<ShaderType Type> auto Shader<Type>::get() const noexcept -> GLuint {
    return id;
}

template class Shader<ShaderType::VERTEX>;
template class Shader<ShaderType::FRAGMENT>;

} // namespace zpg::graphics
