#pragma once
#include "zpg/graphics/MeshData.hpp"
#include <array>

namespace models {

inline constexpr auto plain_data
    = std::to_array({1.0f,  0.0f,  1.0f,  0.02f, 0.20f, 0.06f, 1.0f,  0.0f,
                     -1.0f, 0.02f, 0.20f, 0.06f, -1.0f, 0.0f,  -1.0f, 0.02f,
                     0.20f, 0.06f, -1.0f, 0.0f,  1.0f,  0.02f, 0.20f, 0.06f,
                     1.0f,  0.0f,  1.0f,  0.02f, 0.20f, 0.06f, -1.0f, 0.0f,
                     -1.0f, 0.02f, 0.20f, 0.06f});

inline constexpr zpg::graphics::MeshData plain{
  plain_data, zpg::graphics::VertexLayout::PosNormal
};

} // namespace models
