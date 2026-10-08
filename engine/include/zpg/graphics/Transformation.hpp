#pragma once

#include <glm/ext/matrix_float4x4.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/ext/quaternion_float.hpp>
#include <glm/ext/quaternion_transform.hpp>
#include <glm/ext/vector_float3.hpp>
#include <glm/ext/vector_float4.hpp>

namespace zpg::graphics {
class Transformation {
    glm::quat _rotation{1.f, 0.f, 0.f, 0.f};
    glm::vec3 _scale{1.f};
    glm::vec3 _position{0.f};

  public:
    Transformation() = default;

    [[nodiscard]] auto get_transformation() const noexcept -> glm::mat4;

    auto rotate_x(float rads) noexcept -> void;

    auto rotate_y(float rads) noexcept -> void;

    auto rotate_z(float rads) noexcept -> void;

    auto transform(const glm::vec3& offset) noexcept -> void;

    auto scale(const glm::vec3& scale) noexcept -> void;

    auto scale(float scale) noexcept -> void;
};
} // namespace zpg::graphics
