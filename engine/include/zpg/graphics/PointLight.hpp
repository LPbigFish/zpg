#pragma once

#include <glm/vec3.hpp>

namespace zpg::graphics {

struct PointLight {
    glm::vec3 position{0.f};
    glm::vec3 color{1.f};

    float intensity{1.f};

    float constant{1.f};
    float linear{0.09f};
    float quadratic{0.032f};
};

} // namespace zpg::graphics
