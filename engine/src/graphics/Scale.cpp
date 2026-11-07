#include "zpg/graphics/Scale.hpp"
#include <glm/ext/matrix_transform.hpp>

namespace zpg::graphics {

Scale::Scale(const glm::vec3& scale_vec) {
    _matrix = glm::scale(glm::mat4{1.f}, scale_vec);
}

Scale::Scale(float scale_x, float scale_y, float scale_z):
    Scale(glm::vec3{scale_x, scale_y, scale_z}) {}

Scale::Scale(float scale_vec):
    Scale(glm::vec3{scale_vec, scale_vec, scale_vec}) {}

auto Scale::get_matrix() -> glm::mat4 {
    return _matrix;
}

auto Scale::clone() -> std::unique_ptr<TransformationComponent> {
    return std::make_unique<Scale>(*this);
}

} // namespace zpg::graphics
