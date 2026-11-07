#pragma once

#include "zpg/graphics/VertexArray.hpp"
#include <array>
#include <cstddef>
#include <span>

namespace zpg::graphics {

struct MeshData {
    std::span<const float> vertices;
    VertexLayout layout;
    std::size_t vertex_count;

    template<std::size_t Size>
    constexpr MeshData(
        const std::array<float, Size>& data, const VertexLayout vertex_layout
    ):
        vertices{data}, layout{vertex_layout},
        vertex_count{Size / stride(vertex_layout)} {}

    template<std::size_t Size>
    constexpr MeshData(
        const float (&data)[Size], const VertexLayout vertex_layout
    ):
        vertices{data, Size}, layout{vertex_layout},
        vertex_count{Size / stride(vertex_layout)} {}

    // ponytail: runtime-generated meshes (colored tree/bush); count in floats
    constexpr MeshData(
        std::span<const float> data,
        const VertexLayout vertex_layout,
        const std::size_t float_count
    ):
        vertices{data}, layout{vertex_layout},
        vertex_count{float_count / stride(vertex_layout)} {}

  private:
    static constexpr auto stride(const VertexLayout layout) -> std::size_t {
        switch (layout) {
            case VertexLayout::PosNormalColor:
            case VertexLayout::PosColorNormal: return 9;
            case VertexLayout::PosColor:
            case VertexLayout::PosNormal: return 6;
            case VertexLayout::PosNormalUv: return 8;
            case VertexLayout::Pos: return 3;
        }
        return 1;
    }
};

} // namespace zpg::graphics
