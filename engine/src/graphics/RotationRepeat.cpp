#include "zpg/graphics/RotationRepeat.hpp"
#include "zpg/utils/Time.hpp"
#include <glm/ext/matrix_transform.hpp>
#include <glm/trigonometric.hpp>

namespace zpg::graphics {

RotationRepeat::RotationRepeat(
    const glm::vec3& rotation_vec, float degrees_per_second
):
    Rotation{rotation_vec, 0.f}, _rotation_vec{rotation_vec},
    _degrees_per_second{degrees_per_second} {}

auto RotationRepeat::get_matrix() -> glm::mat4 {
    return glm::rotate(
        glm::mat4{1.f},
        glm::radians(_degrees_per_second) * utils::Time::get_elapsed_time(),
        _rotation_vec
    );
}

auto RotationRepeat::clone() -> std::unique_ptr<TransformationComponent> {
    return std::make_unique<RotationRepeat>(*this);
}

} // namespace zpg::graphics
