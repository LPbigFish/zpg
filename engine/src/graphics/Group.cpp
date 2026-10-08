#include "zpg/graphics/Group.hpp"
#include "zpg/graphics/SceneNode.hpp"
#include <memory>

namespace zpg::graphics {

auto Group::render_in_world(const glm::mat4& mat) -> void {
    for (auto& child : this->_children) {
        child->render(mat);
    }
}

auto Group::add(std::unique_ptr<SceneNode> child) -> void {
    _children.push_back(std::move(child));
}
} // namespace zpg::graphics
