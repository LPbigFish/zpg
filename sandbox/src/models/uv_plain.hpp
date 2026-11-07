#pragma once
#include "zpg/graphics/MeshData.hpp"
#include <array>

namespace models {

inline constexpr std::array<float, 48> uv_plain_data{
  // vrchol, normála, uv souřadnice
  1.0f,  0.0f, 1.0f, 0.0f, 1.0f,  0.0f, 0.0f,  0.0f, 1.0f, 0.0f, -1.0f, 0.0f,
  1.0f,  0.0f, 1.0f, 0.0f, -1.0f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f, 1.0f,  1.0f,
  -1.0f, 0.0f, 1.0f, 0.0f, 1.0f,  0.0f, 0.0f,  1.0f, 1.0f, 0.0f, 1.0f,  0.0f,
  1.0f,  0.0f, 0.0f, 0.0f, -1.0f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f, 1.0f,  1.0f
};

inline constexpr zpg::graphics::MeshData uv_plain{
  uv_plain_data, zpg::graphics::VertexLayout::PosNormalUv
};

} // namespace models
