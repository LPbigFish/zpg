#pragma once

#include "zpg/graphics/VertexBuffer.hpp"
#include <glad/gl.h>
#include <zpg/core/GlType.hpp>

namespace zpg::graphics {

class Model {
    VertexBuffer vertex_buffer;
    GLuint vao{};

  public:
    Model() = delete;

    explicit Model(std::span<const float> data);

    ~Model() {
        if (vao != 0) {
            glDeleteVertexArrays(1, &vao);
        }
    }

    Model(Model&& other) noexcept:
        vertex_buffer{std::move(other.vertex_buffer)},
        vao{std::exchange(other.vao, 0)} {}

    auto operator=(Model&& other) noexcept -> Model& = delete;
    Model(const Model& other) = delete;
    auto operator=(const Model& other) -> Model& = delete;

    [[nodiscard]] auto get_size() const noexcept -> std::size_t {
        return vertex_buffer.get_size();
    }

    [[nodiscard]] auto get_count() const noexcept -> GLsizei {
        return static_cast<GLsizei>(
            vertex_buffer.get_size() / (6 * sizeof(float))
        );
    }

    [[nodiscard]] auto get_vao() const noexcept -> GLuint {
        return vao;
    }

    auto bind() const noexcept -> void {
        glBindVertexArray(vao);
    }

    // NOLINTNEXTLINE
    auto unbind() const noexcept -> void {
        glBindVertexArray(0);
    }
};
} // namespace zpg::graphics
