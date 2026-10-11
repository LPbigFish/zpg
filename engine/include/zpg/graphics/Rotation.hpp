#pragma once

#include "zpg/graphics/TransformationComponent.hpp"
#include <glm/ext/matrix_float4x4.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/ext/vector_float3.hpp>
#include <glm/trigonometric.hpp>
#include <memory>

namespace zpg::graphics {

class Rotation: public TransformationComponent {
    glm::mat4 _matrix{1.f};

  public:
    // Angle is given in DEGREES (converted to radians internally)
    Rotation(const glm::vec3& rotation_vec, float degrees) {
        _matrix
            = glm::rotate(glm::mat4{1.f}, glm::radians(degrees), rotation_vec);
    }

    [[nodiscard]] auto get_matrix() -> glm::mat4 override {
        return _matrix;
    }

    auto change_rotation(const glm::vec3& rotation_vec, float degrees) -> void {
        _matrix
            = glm::rotate(glm::mat4{1.f}, glm::radians(degrees), rotation_vec);
    }

    [[nodiscard]] auto clone()
        -> std::unique_ptr<TransformationComponent> override {
        return std::make_unique<Rotation>(*this);
    }
};
} // namespace zpg::graphics
