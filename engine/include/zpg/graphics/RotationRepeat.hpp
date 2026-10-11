#pragma once

#include "zpg/graphics/Rotation.hpp"
#include "zpg/utils/Time.hpp"
#include <glm/ext/matrix_float4x4.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/ext/vector_float3.hpp>
#include <memory>

namespace zpg::graphics {

class RotationRepeat final: public Rotation {
    glm::vec3 _rotation_vec;
    float _degrees_per_second;

  public:
    RotationRepeat(const glm::vec3& rotation_vec, float degrees_per_second):
        Rotation{rotation_vec, 0.f}, _rotation_vec{rotation_vec},
        _degrees_per_second{degrees_per_second} {}

    [[nodiscard]] auto get_matrix() -> glm::mat4 override {
        return glm::rotate(
            glm::mat4{1.f},
            glm::radians(_degrees_per_second)
                * zpg::utils::Time::get_elapsed_time(),
            _rotation_vec
        );
    }

    [[nodiscard]] auto clone()
        -> std::unique_ptr<TransformationComponent> override {
        return std::make_unique<RotationRepeat>(*this);
    }
};
} // namespace zpg::graphics
