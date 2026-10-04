#include "zpg/graphics/Model.hpp"

namespace zpg::graphics {

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
        6 * sizeof(float),
        // NOLINTNEXTLINE
        reinterpret_cast<GLvoid*>(0)
    );
    glVertexAttribPointer(
        1,
        3,
        core::gl_t<float>::V,
        GL_FALSE,
        6 * sizeof(float),
        // NOLINTNEXTLINE
        reinterpret_cast<GLvoid*>(3 * sizeof(float))
    );
}

} // namespace zpg::graphics
