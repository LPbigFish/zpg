#include "zpg/graphics/SceneNode.hpp"

namespace zpg::graphics {

auto SceneNode::set_draw_callback(
    const std::function<void(SceneNode&)>& callback
) noexcept -> void {
    draw_callback = callback;
}

auto SceneNode::render(const glm::mat4& parent_matrix) -> void {
    draw_callback(*this);
    this->render_in_world(parent_matrix * this->get_transformation());
}
} // namespace zpg::graphics
