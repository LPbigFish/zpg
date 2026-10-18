#pragma once

#include "zpg/graphics/TransformationComponent.hpp"
#include <deque>
#include <memory>

namespace zpg::graphics {

// Composite of the Composite design pattern. Children are stored in the order
// they were added and the resulting matrix is their left-to-right product:
//   matrix = child_1 * child_2 * ... * child_n
class TransformationComposite final: public TransformationComponent {
    std::deque<std::unique_ptr<TransformationComponent>> _children;

  public:
    TransformationComposite() = default;
    ~TransformationComposite() = default;

    TransformationComposite(const TransformationComposite& other);

    auto operator=(const TransformationComposite&)
        -> TransformationComposite& = delete;

    auto add(std::unique_ptr<TransformationComponent> component) -> void;

    auto add_front(std::unique_ptr<TransformationComponent> component) -> void;

    [[nodiscard]] auto get_matrix() -> glm::mat4 override;

    [[nodiscard]] auto get_children()
        -> std::deque<std::unique_ptr<TransformationComponent>>&;

    [[nodiscard]] auto clone()
        -> std::unique_ptr<TransformationComponent> override;
};
} // namespace zpg::graphics
