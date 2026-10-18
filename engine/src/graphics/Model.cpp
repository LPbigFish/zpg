#include "zpg/graphics/Model.hpp"
#include <utility>

namespace zpg::graphics {

Model::Model(VertexArray&& vertex_array):
    vertex_array{std::move(vertex_array)} {}

auto Model::get_count() const noexcept -> GLsizei {
    return vertex_array.get_count();
}

auto Model::bind() const noexcept -> void {
    vertex_array.bind();
}

auto Model::unbind() noexcept -> void {
    VertexArray::unbind();
}

} // namespace zpg::graphics
