#pragma once

#include "zpg/graphics/VertexArray.hpp"
#include <expected>
#include <span>
#include <string>
#include <utility>

namespace zpg::graphics {

class Model {
    VertexArray vertex_array;

    explicit Model(VertexArray&& vertex_array):
        vertex_array{std::move(vertex_array)} {}

  public:
    Model() = delete;
    ~Model() = default;
    Model(Model&& other) noexcept = default;
    Model(const Model&) = delete;
    auto operator=(Model&&) -> Model& = delete;
    auto operator=(const Model&) -> Model& = delete;

    template<VertexLayout Layout>
    static auto create(std::span<const float> data)
        -> std::expected<Model, std::string> {
        auto vertex_array = VertexArray::create(data, Layout);
        if (!vertex_array) {
            return std::unexpected{std::move(vertex_array).error()};
        }
        return Model{std::move(*vertex_array)};
    }

    [[nodiscard]] auto get_count() const noexcept -> GLsizei {
        return vertex_array.get_count();
    }

    auto bind() const noexcept -> void {
        vertex_array.bind();
    }

    static auto unbind() noexcept -> void {
        glBindVertexArray(0);
    }
};
} // namespace zpg::graphics
