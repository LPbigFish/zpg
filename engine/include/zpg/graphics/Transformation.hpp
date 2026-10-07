#pragma once

#include <glm/ext/matrix_float4x4.hpp>

namespace zpg::graphics {
class Transformation {
    glm::mat4 _transform;

  public:
    explicit Transformation(glm::mat4 matrix = glm::mat4{1.f});

    [[nodiscard]] auto get_transformation() -> glm::mat4&;
};
} // namespace zpg::graphics
