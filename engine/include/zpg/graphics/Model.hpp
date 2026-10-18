#pragma once

#include "zpg/graphics/VertexArray.hpp"
#include <expected>
#include <span>
#include <string>
#include <utility>

namespace zpg::graphics {

class Model {
    VertexArray vertex_array;

    explicit Model(VertexArray&& vertex_array);

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

    [[nodiscard]] auto get_count() const noexcept -> GLsizei;

    auto bind() const noexcept -> void;

    static auto unbind() noexcept -> void;
};
} // namespace zpg::graphics
