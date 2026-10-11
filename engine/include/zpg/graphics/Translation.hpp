#pragma once

#include "zpg/graphics/TransformationComponent.hpp"
#include <glm/ext/matrix_float4x4.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/ext/vector_float3.hpp>
#include <memory>

namespace zpg::graphics {

class Translation final: public TransformationComponent {
    glm::mat4 _matrix{1.f};

  public:
    explicit Translation(const glm::vec3& translation_vec) {
        _matrix = glm::translate(glm::mat4{1.f}, translation_vec);
    }

    [[nodiscard]] auto get_matrix() -> glm::mat4 override {
        return _matrix;
    }

    auto change_translation(const glm::vec3& translation_vec) -> void {
        _matrix = glm::translate(glm::mat4{1.f}, translation_vec);
    }

    [[nodiscard]] auto clone()
        -> std::unique_ptr<TransformationComponent> override {
        return std::make_unique<Translation>(*this);
    }
};
} // namespace zpg::graphics
