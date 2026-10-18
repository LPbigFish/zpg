#include "zpg/graphics/Group.hpp"
#include "zpg/graphics/SceneNode.hpp"
#include <memory>
#include <utility>

namespace zpg::graphics {

Group::Group(ProgramObserver program_observer):
    observer{std::move(program_observer)} {}

auto Group::render_in_world(const glm::mat4& mat) -> void {
    for (auto& child : this->_children) {
        child->render(mat);
    }
}

auto Group::add(std::unique_ptr<SceneNode> child) -> SceneNode& {
    auto& result = *child;
    _children.push_back(std::move(child));
    return result;
}

auto Group::add_group() -> Group& {
    return add(std::make_unique<Group>(observer));
}
} // namespace zpg::graphics
