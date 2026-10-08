#pragma once

#include "zpg/graphics/Transformation.hpp"
#include <glm/ext/matrix_float4x4.hpp>

namespace zpg::graphics {

class SceneNode: public Transformation {
    virtual auto render_in_world(const glm::mat4& mat) -> void = 0;

  protected:
    std::function<void(SceneNode&)> draw_callback{[](SceneNode&) -> void {}};

  public:
    auto
    set_draw_callback(const std::function<void(SceneNode&)>& callback) noexcept
        -> void;
    auto render(const glm::mat4& parent_matrix) -> void;
    SceneNode() = default;
    SceneNode(const SceneNode&) = delete;
    auto operator=(const SceneNode&) -> SceneNode& = delete;
    SceneNode(SceneNode&&) = delete;
    auto operator=(SceneNode&&) -> SceneNode& = delete;
    virtual ~SceneNode() = default;
};
} // namespace zpg::graphics
