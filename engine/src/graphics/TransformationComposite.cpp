#include "zpg/graphics/TransformationComposite.hpp"
#include <utility>

namespace zpg::graphics {

TransformationComposite::TransformationComposite(
    const TransformationComposite& other
) {
    for (const auto& child : other._children) {
        _children.push_back(child->clone());
    }
}

auto TransformationComposite::add(
    std::unique_ptr<TransformationComponent> component
) -> void {
    _children.push_back(std::move(component));
}

auto TransformationComposite::add_front(
    std::unique_ptr<TransformationComponent> component
) -> void {
    _children.push_front(std::move(component));
}

auto TransformationComposite::get_matrix() -> glm::mat4 {
    auto result = glm::mat4{1.f};
    for (const auto& transformation : _children) {
        result *= transformation->get_matrix();
    }
    return result;
}

auto TransformationComposite::get_children()
    -> std::deque<std::unique_ptr<TransformationComponent>>& {
    return _children;
}

auto TransformationComposite::clone()
    -> std::unique_ptr<TransformationComponent> {
    return std::make_unique<TransformationComposite>(*this);
}

} // namespace zpg::graphics
