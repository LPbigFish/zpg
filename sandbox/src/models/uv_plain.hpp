#pragma once
#include <array>

constexpr std::array<float, 48> uv_plain{
  // vrchol, normála, uv souřadnice
  1.0f,  0.0f, 1.0f, 0.0f, 1.0f,  0.0f, 0.0f,  0.0f, 1.0f, 0.0f, -1.0f, 0.0f,
  1.0f,  0.0f, 1.0f, 0.0f, -1.0f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f, 1.0f,  1.0f,
  -1.0f, 0.0f, 1.0f, 0.0f, 1.0f,  0.0f, 0.0f,  1.0f, 1.0f, 0.0f, 1.0f,  0.0f,
  1.0f,  0.0f, 0.0f, 0.0f, -1.0f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f, 1.0f,  1.0f
};
