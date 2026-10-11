#pragma once

#include "zpg/graphics/TransformationComponent.hpp"
#include <glm/ext/matrix_float4x4.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/ext/vector_float3.hpp>
#include <memory>

namespace zpg::graphics {

class Scale final: public TransformationComponent {
    glm::mat4 _matrix{1.f};

  public:
    explicit Scale(const glm::vec3& scale_vec) {
        _matrix = glm::scale(glm::mat4{1.f}, scale_vec);
    }

    explicit Scale(float scale_vec):
        Scale(glm::vec3{scale_vec, scale_vec, scale_vec}) {}

    [[nodiscard]] auto get_matrix() -> glm::mat4 override { return _matrix; }

    [[nodiscard]] auto clone()
        -> std::unique_ptr<TransformationComponent> override {
        return std::make_unique<Scale>(*this);
    }
};
} // namespace zpg::graphics
