#pragma once

#include <glm/vec3.hpp>

namespace zpg::graphics {

struct Material {
    glm::vec3 ambient{0.1f};
    glm::vec3 diffuse{1.f};
    glm::vec3 specular{0.5f};

    float shininess{32.f};
};

} // namespace zpg::graphics
