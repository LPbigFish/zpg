#include "zpg/graphics/Transformation.hpp"

namespace zpg::graphics {
Transformation::Transformation(glm::mat4 matrix): _transform{matrix} {}

[[nodiscard]] auto Transformation::get_transformation() -> glm::mat4& {
    return this->_transform;
}
} // namespace zpg::graphics
