#include "zpg/graphics/SceneNode.hpp"

namespace zpg::graphics {

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

auto SceneNode::render(const glm::mat4& parent_matrix) -> void {
    draw_callback(*this);
    const auto local = node_transform != nullptr
                         ? node_transform->get_matrix()
                         : Transformation::get_transformation();
    this->render_in_world(parent_matrix * local);
}
} // namespace zpg::graphics
