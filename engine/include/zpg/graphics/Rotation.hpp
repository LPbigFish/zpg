#pragma once

#include "zpg/graphics/TransformationComponent.hpp"
#include <glm/ext/matrix_float4x4.hpp>
#include <glm/ext/vector_float3.hpp>
#include <memory>

namespace zpg::graphics {

class Rotation: public TransformationComponent {
    glm::mat4 _matrix{1.f};

    class Axis {
        char axis;

      public:
        // NOLINTNEXTLINE(google-explicit-constructor)
        consteval Axis(char x): axis{x} {
            if (axis != 'x' && axis != 'y' && axis != 'z') {
                throw std::invalid_argument{
                  "Axis must be one of 'x', 'y', or 'z'"
                };
            }
        }

        [[nodiscard]] auto get_axis() const -> glm::vec3 {
            switch (axis) {
                case 'x': return glm::vec3{1.f, 0.f, 0.f};
                case 'y': return glm::vec3{0.f, 1.f, 0.f};
                case 'z': return glm::vec3{0.f, 0.f, 1.f};
                default: throw std::logic_error{"Invalid axis"};
            }
        }
    };

  public:
    // converted to radians internally
    explicit Rotation(const glm::vec3& rotation_vec, float degrees);

    explicit Rotation(
        float rotation_x, float rotation_y, float rotation_z, float degrees
    );

    explicit Rotation(Axis direction, float degrees);

    [[nodiscard]] auto get_matrix() -> glm::mat4 override;

    auto change_rotation(const glm::vec3& rotation_vec, float degrees) -> void;

    [[nodiscard]] auto clone()
        -> std::unique_ptr<TransformationComponent> override;
};
} // namespace zpg::graphics
