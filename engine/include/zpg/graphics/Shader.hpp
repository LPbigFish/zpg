#pragma once
#include <cstdint>
#include <filesystem>
#include <format>
#include <fstream>
#include <glad/gl.h>
#include <iterator>
#include <stdexcept>
#include <utility>

namespace fs = std::filesystem;

namespace zpg::graphics {

enum class ShaderType : uint8_t {
    VERTEX,
    FRAGMENT,
};

template<ShaderType Type> class Shader {
    GLuint id{};

    static constexpr auto gl_type() noexcept -> GLenum {
        if constexpr (Type == ShaderType::VERTEX) {
            return GL_VERTEX_SHADER;
        } else if constexpr (Type == ShaderType::FRAGMENT) {
            return GL_FRAGMENT_SHADER;
        }
    }

    static constexpr auto shader_name() noexcept -> std::string_view {
        if constexpr (Type == ShaderType::VERTEX) {
            return "Vertex";
        } else if constexpr (Type == ShaderType::FRAGMENT) {
            return "Fragment";
        }
    }

  public:
    explicit Shader(const fs::path& path) {
        // MARK: FILESYSTEM

        if (!fs::exists(path)) {
            throw std::runtime_error{
              std::format("Shader not found on path: {}", path.string())
            };
        }

        std::ifstream file{path};
        std::string shader_code{
          std::istreambuf_iterator<char>{file}, std::istreambuf_iterator<char>{}
        };
        const auto* c_sc = shader_code.c_str();

        // MARK: SHADER CREATION

        id = glCreateShader(gl_type());

        if (id == 0) {
            throw std::runtime_error{
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
            id = 0;

            throw std::runtime_error{std::format(
                "Failed to compile {} shader: {}", shader_name(), info_log
            )};
        }
    }

    Shader(Shader&& other) noexcept: id{std::exchange(other.id, 0)} {}

    ~Shader() {
        glDeleteShader(id);
    }

    [[nodiscard]] auto get() const noexcept -> GLuint {
        return id;
    }

    Shader() = delete;
    Shader(const Shader&) = delete;
    auto operator=(const Shader&) -> Shader& = delete;
    auto operator=(Shader&&) -> Shader& = delete;
};

using VertexShader = Shader<ShaderType::VERTEX>;
using FragmentShader = Shader<ShaderType::FRAGMENT>;

} // namespace zpg::graphics
