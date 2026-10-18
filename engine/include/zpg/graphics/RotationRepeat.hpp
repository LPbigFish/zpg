#pragma once

#include "zpg/graphics/Rotation.hpp"
#include <glm/ext/matrix_float4x4.hpp>
#include <glm/ext/vector_float3.hpp>
#include <memory>

namespace zpg::graphics {

class RotationRepeat final: public Rotation {
    glm::vec3 _rotation_vec;
    float _degrees_per_second;

  public:
    RotationRepeat(const glm::vec3& rotation_vec, float degrees_per_second);

    [[nodiscard]] auto get_matrix() -> glm::mat4 override;

    [[nodiscard]] auto clone()
        -> std::unique_ptr<TransformationComponent> override;
};
} // namespace zpg::graphics
