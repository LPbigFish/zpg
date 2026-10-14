#pragma once

#include "zpg/graphics/Transformation.hpp"
#include <glm/ext/matrix_float4x4.hpp>

namespace zpg::core {
class Camera: graphics::Transformation {
    glm::mat4 viewMatrix{1};
    glm::mat4 projectionMatrix{1};

  public:
    Camera();
    [[nodiscard]] auto get_camera() const -> glm::mat4;
};
} // namespace zpg::core
