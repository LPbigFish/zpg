#pragma once

#include "zpg/graphics/TransformationComponent.hpp"
#include <deque>
#include <memory>
#include <utility>

namespace zpg::graphics {

// Composite of the Composite design pattern. Children are stored in the order
// they were added and the resulting matrix is their left-to-right product:
//   matrix = child_1 * child_2 * ... * child_n
class TransformationComposite final: public TransformationComponent {
    std::deque<std::unique_ptr<TransformationComponent>> _children;

  public:
    TransformationComposite() = default;
    ~TransformationComposite() = default;

    TransformationComposite(const TransformationComposite& other) {
        for (const auto& child : other._children) {
            _children.push_back(child->clone());
        }
    }

    auto operator=(const TransformationComposite&)
        -> TransformationComposite& = delete;

    auto add(std::unique_ptr<TransformationComponent> component) -> void {
        _children.push_back(std::move(component));
    }

    auto add_front(std::unique_ptr<TransformationComponent> component) -> void {
        _children.push_front(std::move(component));
    }

    [[nodiscard]] auto get_matrix() -> glm::mat4 override {
        auto result = glm::mat4{1.f};
        for (const auto& transformation : _children) {
            result *= transformation->get_matrix();
        }
        return result;
    }

    [[nodiscard]] auto get_children()
        -> std::deque<std::unique_ptr<TransformationComponent>>& {
        return _children;
    }

    [[nodiscard]] auto clone()
        -> std::unique_ptr<TransformationComponent> override {
        return std::make_unique<TransformationComposite>(*this);
    }
};
} // namespace zpg::graphics
