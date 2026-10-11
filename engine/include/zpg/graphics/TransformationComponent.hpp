#pragma once

#include <glm/ext/matrix_float4x4.hpp>
#include <memory>

namespace zpg::graphics {
class TransformationComponent {
  public:
    TransformationComponent() = default;
    virtual ~TransformationComponent() = default;
    TransformationComponent(const TransformationComponent&) = default;
    auto operator=(const TransformationComponent&)
        -> TransformationComponent& = default;
    TransformationComponent(TransformationComponent&&) = delete;
    auto operator=(TransformationComponent&&)
        -> TransformationComponent& = delete;

    [[nodiscard]] virtual auto get_matrix() -> glm::mat4 = 0;

    [[nodiscard]] virtual auto clone()
        -> std::unique_ptr<TransformationComponent>
        = 0;
};
} // namespace zpg::graphics
