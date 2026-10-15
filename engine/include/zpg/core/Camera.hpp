#pragma once
#include <glm/mat4x4.hpp>

namespace zpg::core {
class Camera {
    glm::mat4 viewMatrix{1};
    glm::mat4 projectionMatrix{1};

  public:
    Camera();
    [[nodiscard]] auto get_camera() const -> glm::mat4;
};
} // namespace zpg::core
