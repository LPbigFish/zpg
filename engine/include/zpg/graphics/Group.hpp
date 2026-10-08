#pragma once

#include "zpg/graphics/SceneNode.hpp"
#include <memory>
#include <vector>

namespace zpg::graphics {
class Group final: public SceneNode {
    std::vector<std::unique_ptr<SceneNode>> _children;

    auto render_in_world(const glm::mat4& mat) -> void override;

  public:
    auto add(std::unique_ptr<SceneNode> child) -> void;
};
} // namespace zpg::graphics
