#pragma once

#include "zpg/graphics/TransformationComponent.hpp"
#include <glm/ext/matrix_float4x4.hpp>
#include <glm/ext/vector_float3.hpp>
#include <memory>

namespace zpg::graphics {

class Rotation: public TransformationComponent {
    glm::mat4 _matrix{1.f};

  public:
    // Angle is given in DEGREES (converted to radians internally)
    Rotation(const glm::vec3& rotation_vec, float degrees);

    [[nodiscard]] auto get_matrix() -> glm::mat4 override;

    auto change_rotation(const glm::vec3& rotation_vec, float degrees) -> void;

    [[nodiscard]] auto clone()
        -> std::unique_ptr<TransformationComponent> override;
};
} // namespace zpg::graphics
