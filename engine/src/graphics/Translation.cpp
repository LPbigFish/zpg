#include "zpg/graphics/Translation.hpp"
#include <glm/ext/matrix_transform.hpp>

namespace zpg::graphics {

Translation::Translation(const glm::vec3& translation_vec) {
    _matrix = glm::translate(glm::mat4{1.f}, translation_vec);
}

Translation::Translation(
    float translation_x, float translation_y, float translation_z
) {
    _matrix = glm::translate(
        glm::mat4{1.f}, glm::vec3{translation_x, translation_y, translation_z}
    );
}

auto Translation::get_matrix() -> glm::mat4 {
    return _matrix;
}

auto Translation::change_translation(const glm::vec3& translation_vec) -> void {
    _matrix = glm::translate(glm::mat4{1.f}, translation_vec);
}

auto Translation::clone() -> std::unique_ptr<TransformationComponent> {
    return std::make_unique<Translation>(*this);
}

} // namespace zpg::graphics
