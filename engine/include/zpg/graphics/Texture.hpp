#pragma once
#include <expected>
#include <glad/gl.h>
#include <string>

namespace zpg::graphics {
class Texture {
    GLuint id{};

    explicit Texture(GLuint id);

  public:
    Texture() = delete;
    Texture(Texture&& other) noexcept;
    ~Texture();

    Texture(const Texture&) = delete;
    auto operator=(const Texture&) -> Texture& = delete;
    auto operator=(Texture&&) -> Texture& = delete;

    static auto create(const std::string& path)
        -> std::expected<Texture, std::string>;

    [[nodiscard]] auto get_id() const noexcept -> GLuint;
};

} // namespace zpg::graphics
