#include "zpg/graphics/SceneNode.hpp"

namespace zpg::graphics {

SceneNode::SceneNode(): draw_callback{[](SceneNode&) -> void {}} {}

auto SceneNode::is_node_transform_active() const noexcept -> bool {
    return node_transform != nullptr;
}

auto SceneNode::set_draw_callback(
    const std::function<void(SceneNode&)>& callback
) noexcept -> void {
    draw_callback = callback;
}

auto SceneNode::set_transform(
    std::shared_ptr<TransformationComponent> transformation
) -> SceneNode& {
    node_transform = std::move(transformation);
    return *this;
}

auto SceneNode::add_transformation(
    std::unique_ptr<TransformationComponent> component
) -> SceneNode& {
    if (_transformation->get_children().empty()
        && !is_node_transform_active()) {
        set_transform(_transformation);
    }
    _transformation->add(std::move(component));
    return *this;
}

auto SceneNode::add_transformation_front(
    std::unique_ptr<TransformationComponent> component
) -> SceneNode& {
    if (_transformation->get_children().empty()
        && !is_node_transform_active()) {
        set_transform(_transformation);
    }
    _transformation->add_front(std::move(component));
    return *this;
}

auto SceneNode::get_transformation_composite() const
    -> const std::shared_ptr<TransformationComposite>& {
    return _transformation;
}

auto SceneNode::move_transform_from(SceneNode& other) noexcept -> void {
    node_transform = std::move(other.node_transform);
    _transformation = std::move(other._transformation);
}

auto SceneNode::render(const glm::mat4& parent_matrix) -> void {
    draw_callback(*this);
    const auto local = node_transform != nullptr
                         ? node_transform->get_matrix()
                         : Transformation::get_transformation();
    this->render_in_world(parent_matrix * local);
}
} // namespace zpg::graphics
