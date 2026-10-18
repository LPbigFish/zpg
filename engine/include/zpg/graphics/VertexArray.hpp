#pragma once

#include "zpg/graphics/VertexBuffer.hpp"
#include <cstddef>
#include <cstdint>
#include <expected>
#include <glad/gl.h>
#include <glm/ext/vector_float3.hpp>
#include <span>
#include <string>
#include <vector>

namespace zpg::graphics {

enum class VertexLayout : std::uint8_t {
    PosNormalColor, // 9 floats: position, normal, color
    PosColorNormal, // 9 floats: position, color, normal
    PosColor,       // 6 floats: position, color
    PosNormal,      // 6 floats: position, normal
};

struct Vertex {
    glm::vec3 position;
    glm::vec3 color;
    glm::vec3 normal;
};

class VertexArray {
    VertexBuffer vertex_buffer;
    GLuint vao{};

    VertexArray(GLuint vao, VertexBuffer&& vertex_buffer);

    [[nodiscard]] static auto
    to_vertices(std::span<const float> data, VertexLayout layout)
        -> std::vector<Vertex>;

    static auto setup_attributes() -> void;

  public:
    static auto create(std::span<const float> data, VertexLayout layout)
        -> std::expected<VertexArray, std::string>;

    template<VertexLayout Layout>
    static auto create(std::span<const float> data)
        -> std::expected<VertexArray, std::string> {
        return create(data, Layout);
    }

    VertexArray(VertexArray&& other) noexcept;

    ~VertexArray();

    VertexArray() = delete;
    VertexArray(const VertexArray&) = delete;
    auto operator=(const VertexArray&) -> VertexArray& = delete;
    auto operator=(VertexArray&&) -> VertexArray& = delete;

    [[nodiscard]] auto get_count() const noexcept -> GLsizei;

    auto bind() const noexcept -> void;

    // NOLINTNEXTLINE
    static auto unbind() noexcept -> void;
};

} // namespace zpg::graphics
