#pragma once

#include <glad/gl.h>
#include <span>
#include <type_traits>

namespace zpg::graphics {

class VertexBuffer {
    GLuint id{};
    GLsizei size{};

    VertexBuffer(const void* data, GLsizei size);

  public:
    VertexBuffer() = delete;

    template<typename T>
        requires std::is_trivially_copyable_v<T>
    explicit VertexBuffer(std::span<const T> data):
        VertexBuffer(data.data(), static_cast<GLsizei>(data.size_bytes())) {}

    ~VertexBuffer();

    VertexBuffer(VertexBuffer&& other) noexcept;

    auto operator=(VertexBuffer&& other) noexcept -> VertexBuffer& = delete;
    VertexBuffer(const VertexBuffer& other) = delete;
    auto operator=(const VertexBuffer& other) -> VertexBuffer& = delete;

    [[nodiscard]] auto get() const noexcept -> GLuint;

    [[nodiscard]] auto get_size() const noexcept -> GLsizeiptr;

    auto bind() const noexcept -> void;

    static auto unbind() noexcept -> void;
};

} // namespace zpg::graphics
