#include "zpg/graphics/Texture.hpp"
#include <utility>
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

namespace zpg::graphics {

Texture::Texture(GLuint id): id{id} {}

Texture::Texture(Texture&& other) noexcept: id{std::exchange(other.id, 0)} {}

Texture::~Texture() {
    if (id != 0) {
        glDeleteTextures(1, &id);
    }
}

auto Texture::get_id() const noexcept -> GLuint {
    return id;
}

auto Texture::create(const std::string& path)
    -> std::expected<Texture, std::string> {
    int width{};
    int height{};

    stbi_set_flip_vertically_on_load(1);
    unsigned char* data
        = stbi_load(path.c_str(), &width, &height, nullptr, STBI_rgb_alpha);
    if (!data) {
        return std::unexpected{"Failed to load texture from path: " + path};
    }

    GLuint texture_id{};
    glGenTextures(1, &texture_id);
    if (texture_id == 0) {
        stbi_image_free(data);
        return std::unexpected{"Failed to generate OpenGL texture"};
    }

    glBindTexture(GL_TEXTURE_2D, texture_id);

    GLint unpack_alignment{};
    glGetIntegerv(GL_UNPACK_ALIGNMENT, &unpack_alignment);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glTexImage2D(
        GL_TEXTURE_2D,
        0,
        GL_RGBA,
        width,
        height,
        0,
        GL_RGBA,
        GL_UNSIGNED_BYTE,
        data
    );
    glPixelStorei(GL_UNPACK_ALIGNMENT, unpack_alignment);
    glGenerateMipmap(GL_TEXTURE_2D);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(
        GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR
    );
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    stbi_image_free(data);

    return Texture{texture_id};
}

} // namespace zpg::graphics
