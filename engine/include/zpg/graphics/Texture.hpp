#pragma once
#include <array>
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

    static auto create_cubemap(const std::string paths[6])
        -> std::expected<Texture, std::string>;

    static auto create_cube_map(const std::array<std::string, 6>& faces)
        -> std::expected<Texture, std::string>;

    [[nodiscard]] auto get_id() const noexcept -> GLuint;
};

} // namespace zpg::graphics
