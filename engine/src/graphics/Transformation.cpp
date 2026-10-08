#include "zpg/graphics/Transformation.hpp"
#include <glm/ext/quaternion_float.hpp>
#include <glm/ext/quaternion_geometric.hpp>
#include <glm/ext/quaternion_transform.hpp>
#include <glm/ext/quaternion_trigonometric.hpp>
#include <glm/ext/vector_float3.hpp>
#include <glm/gtc/quaternion.hpp>

namespace zpg::graphics {

[[nodiscard]] auto Transformation::get_transformation() const noexcept
    -> glm::mat4 {
    const auto translation = glm::translate(glm::mat4{1.f}, _position);

    const auto rotation = glm::mat4_cast(_rotation);

    const auto scale = glm::scale(glm::mat4{1.f}, _scale);

    return translation * rotation * scale;
}

auto Transformation::transform(const glm::vec3& offset) noexcept -> void {
    _position += offset;
}

auto Transformation::scale(const glm::vec3& scale) noexcept -> void {
    _scale *= scale;
}

auto Transformation::scale(float scale) noexcept -> void {
    _scale *= scale;
}

auto Transformation::rotate_x(float rads) noexcept -> void {
    _rotation *= glm::angleAxis(rads, glm::vec3{1.f, 0.f, 0.f});
    _rotation = glm::normalize(_rotation);
}

auto Transformation::rotate_y(float rads) noexcept -> void {
    _rotation *= glm::angleAxis(rads, glm::vec3{0.f, 1.f, 0.f});
    _rotation = glm::normalize(_rotation);
}

auto Transformation::rotate_z(float rads) noexcept -> void {
    _rotation *= glm::angleAxis(rads, glm::vec3{0.f, 0.f, 1.f});
    _rotation = glm::normalize(_rotation);
}
} // namespace zpg::graphics
