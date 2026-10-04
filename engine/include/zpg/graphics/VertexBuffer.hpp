#pragma once

#include <glad/gl.h>
#include <span>
#include <type_traits>
#include <utility>

namespace zpg::graphics {

class VertexBuffer {
    GLuint id{};
    GLsizei size{};

  public:
    VertexBuffer() = delete;

    template<typename T>
        requires std::is_trivially_copyable_v<T>
    explicit VertexBuffer(std::span<const T> data):
        size(static_cast<GLsizei>(data.size_bytes())) {
        glGenBuffers(1, &id);
        glBindBuffer(GL_ARRAY_BUFFER, id);
        glBufferData(GL_ARRAY_BUFFER, size, data.data(), GL_STATIC_DRAW);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }

    ~VertexBuffer() {
        if (id != 0) {
            glDeleteBuffers(1, &id);
        }
    }

    VertexBuffer(VertexBuffer&& other) noexcept:
        id{std::exchange(other.id, 0)},
        size{std::exchange(other.size, 0)} {}

    auto operator=(VertexBuffer&& other) noexcept -> VertexBuffer& = delete;
    VertexBuffer(const VertexBuffer& other) = delete;
    auto operator=(const VertexBuffer& other) -> VertexBuffer& = delete;

    [[nodiscard]] auto get() const noexcept -> GLuint {
        return id;
    }

    [[nodiscard]] auto get_size() const noexcept -> GLsizeiptr {
        return size;
    }

    auto bind() const noexcept -> void {
        glBindBuffer(GL_ARRAY_BUFFER, id);
    }

    static auto unbind() noexcept -> void {
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }
};

} // namespace zpg::graphics
