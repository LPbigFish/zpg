#include "zpg/graphics/VertexBuffer.hpp"
#include <utility>

namespace zpg::graphics {

VertexBuffer::VertexBuffer(const void* data, GLsizei size): size{size} {
    glGenBuffers(1, &id);
    glBindBuffer(GL_ARRAY_BUFFER, id);
    glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

VertexBuffer::~VertexBuffer() {
    if (id != 0) {
        glDeleteBuffers(1, &id);
    }
}

VertexBuffer::VertexBuffer(VertexBuffer&& other) noexcept:
    id{std::exchange(other.id, 0)}, size{std::exchange(other.size, 0)} {}

auto VertexBuffer::get() const noexcept -> GLuint {
    return id;
}

auto VertexBuffer::get_size() const noexcept -> GLsizeiptr {
    return size;
}

auto VertexBuffer::bind() const noexcept -> void {
    glBindBuffer(GL_ARRAY_BUFFER, id);
}

auto VertexBuffer::unbind() noexcept -> void {
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

} // namespace zpg::graphics
