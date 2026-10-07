#include "zpg/graphics/Model.hpp"
#include <utility>

namespace zpg::graphics {

Model::~Model() {
    if (vao != 0) {
        glDeleteVertexArrays(1, &vao);
    }
}

Model::Model(Model&& other) noexcept:
    vertex_buffer{std::move(other.vertex_buffer)},
    vao{std::exchange(other.vao, 0)} {}

Model::Model(std::span<const float> data): vertex_buffer{data} {
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    vertex_buffer.bind();

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(
        0,
        3,
        core::gl_t<float>::V,
        GL_FALSE,
        9 * sizeof(float),
        // NOLINTNEXTLINE
        reinterpret_cast<GLvoid*>(0)
    );
    glVertexAttribPointer(
        1,
        3,
        core::gl_t<float>::V,
        GL_FALSE,
        9 * sizeof(float),
        // NOLINTNEXTLINE
        reinterpret_cast<GLvoid*>(3 * sizeof(float))
    );
    glVertexAttribPointer(
        2,
        3,
        core::gl_t<float>::V,
        GL_FALSE,
        9 * sizeof(float),
        // NOLINTNEXTLINE
        reinterpret_cast<GLvoid*>(6 * sizeof(float))
    );
}

} // namespace zpg::graphics
