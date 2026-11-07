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

auto Texture::create_cubemap(const std::string paths[6])
    -> std::expected<Texture, std::string> {
    GLuint texture_id{};
    glGenTextures(1, &texture_id);
    if (texture_id == 0) {
        return std::unexpected{"Failed to generate OpenGL cubemap"};
    }

    glBindTexture(GL_TEXTURE_CUBE_MAP, texture_id);

    stbi_set_flip_vertically_on_load(0);
    for (int face = 0; face < 6; ++face) {
        int width{};
        int height{};
        unsigned char* data = stbi_load(
            paths[face].c_str(), &width, &height, nullptr, STBI_rgb_alpha
        );
        if (!data) {
            glDeleteTextures(1, &texture_id);
            return std::unexpected{
              "Failed to load cubemap face: " + paths[face]
            };
        }

        glTexImage2D(
            GL_TEXTURE_CUBE_MAP_POSITIVE_X + face,
            0,
            GL_RGBA,
            width,
            height,
            0,
            GL_RGBA,
            GL_UNSIGNED_BYTE,
            data
        );
        stbi_image_free(data);
    }

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    return Texture{texture_id};
}

auto Texture::create_cube_map(const std::array<std::string, 6>& faces)
    -> std::expected<Texture, std::string> {
    GLuint id{};
    glGenTextures(1, &id);

    if (id == 0) {
        return std::unexpected{"Cannot create CubeMap"};
    }

    glBindTexture(GL_TEXTURE_CUBE_MAP, id);

    stbi_set_flip_vertically_on_load(0);

    int face_width{};
    int face_height{};

    for (std::size_t i = 0; i < faces.size(); ++i) {
        int width{};
        int height{};
        int channels{};

        auto* data = stbi_load(
            faces.at(i).c_str(), &width, &height, &channels, STBI_rgb_alpha
        );

        if (!data || width != height
            || (i != 0 && (width != face_width || height != face_height))) {
            stbi_image_free(data);
            glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
            glDeleteTextures(1, &id);
            stbi_set_flip_vertically_on_load(1);

            return std::unexpected{"Invalid CubeMap face: " + faces.at(i)};
        }

        if (i == 0) {
            face_width = width;
            face_height = height;
        }

        glTexImage2D(
            GL_TEXTURE_CUBE_MAP_POSITIVE_X + static_cast<GLenum>(i),
            0,
            GL_RGBA8,
            width,
            height,
            0,
            GL_RGBA,
            GL_UNSIGNED_BYTE,
            data
        );

        stbi_image_free(data);
    }

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    for (GLenum axis :
         {GL_TEXTURE_WRAP_S, GL_TEXTURE_WRAP_T, GL_TEXTURE_WRAP_R}) {
        glTexParameteri(GL_TEXTURE_CUBE_MAP, axis, GL_CLAMP_TO_EDGE);
    }

    glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

    stbi_set_flip_vertically_on_load(1);

    return Texture{id};
}

} // namespace zpg::graphics
