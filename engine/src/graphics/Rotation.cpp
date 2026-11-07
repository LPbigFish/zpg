#include "zpg/graphics/Rotation.hpp"
#include <glm/ext/matrix_transform.hpp>
#include <glm/trigonometric.hpp>

namespace zpg::graphics {

Rotation::Rotation(const glm::vec3& rotation_vec, float degrees) {
    _matrix = glm::rotate(glm::mat4{1.f}, glm::radians(degrees), rotation_vec);
}

Rotation::Rotation(
    float rotation_x, float rotation_y, float rotation_z, float degrees
) {
    _matrix = glm::rotate(
        glm::mat4{1.f},
        glm::radians(degrees),
        glm::vec3{rotation_x, rotation_y, rotation_z}
    );
}

Rotation::Rotation(Axis direction, float degrees) {
    _matrix = glm::rotate(
        glm::mat4{1.f}, glm::radians(degrees), direction.get_axis()
    );
}

auto Rotation::get_matrix() -> glm::mat4 {
    return _matrix;
}

auto Rotation::change_rotation(const glm::vec3& rotation_vec, float degrees)
    -> void {
    _matrix = glm::rotate(glm::mat4{1.f}, glm::radians(degrees), rotation_vec);
}

auto Rotation::clone() -> std::unique_ptr<TransformationComponent> {
    return std::make_unique<Rotation>(*this);
}

} // namespace zpg::graphics
