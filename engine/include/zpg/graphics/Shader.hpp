#pragma once
#include <cstdint>
#include <expected>
#include <filesystem>
#include <glad/gl.h>
#include <string>
#include <string_view>

namespace fs = std::filesystem;

namespace zpg::graphics {

enum class ShaderType : uint8_t {
    VERTEX,
    FRAGMENT,
};

template<ShaderType Type> class Shader {
    GLuint id{};

    static auto gl_type() noexcept -> GLenum;
    static auto shader_name() noexcept -> std::string_view;

    explicit Shader(GLuint _id): id{_id} {}

  public:
    static auto create(const fs::path& path)
        -> std::expected<Shader, std::string>;

    Shader(Shader&& other) noexcept;

    ~Shader();

    [[nodiscard]] auto get() const noexcept -> GLuint;

    Shader() = delete;
    Shader(const Shader&) = delete;
    auto operator=(const Shader&) -> Shader& = delete;
    auto operator=(Shader&&) -> Shader& = delete;
};

using VertexShader = Shader<ShaderType::VERTEX>;
using FragmentShader = Shader<ShaderType::FRAGMENT>;

} // namespace zpg::graphics
