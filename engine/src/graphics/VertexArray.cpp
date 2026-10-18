#include "zpg/graphics/VertexArray.hpp"
#include "zpg/core/GlType.hpp"
#include <cstddef>
#include <glm/common.hpp>
#include <utility>

namespace zpg::graphics {

VertexArray::VertexArray(GLuint vao, VertexBuffer&& vertex_buffer):
    vertex_buffer{std::move(vertex_buffer)}, vao{vao} {}

auto VertexArray::to_vertices(std::span<const float> data, VertexLayout layout)
    -> std::vector<Vertex> {
    const std::size_t stride = layout == VertexLayout::PosColorNormal
                                    || layout == VertexLayout::PosNormalColor
                                 ? 9
                                 : 6;

    std::vector<Vertex> vertices;
    vertices.reserve(data.size() / stride);
    for (std::size_t i = 0; i + stride <= data.size(); i += stride) {
        glm::vec3 position{data[i], data[i + 1], data[i + 2]};
        glm::vec3 color{1.0f};
        glm::vec3 normal{0.0f};

        if (layout == VertexLayout::PosNormalColor) {
            normal = glm::vec3{data[i + 3], data[i + 4], data[i + 5]};
            color = glm::vec3{data[i + 6], data[i + 7], data[i + 8]};
        } else if (layout == VertexLayout::PosColorNormal) {
            color = glm::vec3{data[i + 3], data[i + 4], data[i + 5]};
            normal = glm::vec3{data[i + 6], data[i + 7], data[i + 8]};
        } else if (layout == VertexLayout::PosColor) {
            color = glm::vec3{data[i + 3], data[i + 4], data[i + 5]};
        } else {
            normal = glm::vec3{data[i + 3], data[i + 4], data[i + 5]};
            color = glm::abs(normal);
        }

        vertices.push_back(
            Vertex{.position = position, .color = color, .normal = normal}
        );
    }
    return vertices;
}

auto VertexArray::setup_attributes() -> void {
    static_assert(sizeof(Vertex) == 9 * sizeof(float));
    constexpr GLsizei STRIDE_BYTES = sizeof(Vertex);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(
        0,
        3,
        core::gl_t<float>::V,
        GL_FALSE,
        STRIDE_BYTES,
        // NOLINTNEXTLINE
        reinterpret_cast<GLvoid*>(offsetof(Vertex, position))
    );
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(
        1,
        3,
        core::gl_t<float>::V,
        GL_FALSE,
        STRIDE_BYTES,
        // NOLINTNEXTLINE
        reinterpret_cast<GLvoid*>(offsetof(Vertex, color))
    );
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(
        2,
        3,
        core::gl_t<float>::V,
        GL_FALSE,
        STRIDE_BYTES,
        // NOLINTNEXTLINE
        reinterpret_cast<GLvoid*>(offsetof(Vertex, normal))
    );
}

auto VertexArray::create(std::span<const float> data, VertexLayout layout)
    -> std::expected<VertexArray, std::string> {
    std::vector<Vertex> vertices = to_vertices(data, layout);
    if (vertices.empty()) {
        return std::unexpected{"Model data contains no vertices"};
    }

    VertexBuffer vertex_buffer{std::span<const Vertex>{vertices}};

    GLuint vao{};
    glGenVertexArrays(1, &vao);
    if (vao == 0) {
        return std::unexpected{"Failed to create vertex array"};
    }

    glBindVertexArray(vao);
    vertex_buffer.bind();
    setup_attributes();
    glBindVertexArray(0);
    VertexBuffer::unbind();

    return VertexArray{vao, std::move(vertex_buffer)};
}

VertexArray::VertexArray(VertexArray&& other) noexcept:
    vertex_buffer{std::move(other.vertex_buffer)},
    vao{std::exchange(other.vao, 0)} {}

VertexArray::~VertexArray() {
    if (vao != 0) {
        glDeleteVertexArrays(1, &vao);
    }
}

auto VertexArray::get_count() const noexcept -> GLsizei {
    return static_cast<GLsizei>(vertex_buffer.get_size() / sizeof(Vertex));
}

auto VertexArray::bind() const noexcept -> void {
    glBindVertexArray(vao);
}

// NOLINTNEXTLINE
auto VertexArray::unbind() noexcept -> void {
    glBindVertexArray(0);
}

} // namespace zpg::graphics
