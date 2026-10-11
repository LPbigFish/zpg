#pragma once

#include "zpg/graphics/Transformation.hpp"
#include "zpg/graphics/TransformationComponent.hpp"
#include <glm/ext/matrix_float4x4.hpp>
#include <memory>

namespace zpg::graphics {

class SceneNode: public Transformation {
    std::shared_ptr<TransformationComponent> node_transform{nullptr};

    std::function<void(SceneNode&)> draw_callback{[](SceneNode&) -> void {}};

    virtual auto render_in_world(const glm::mat4& mat) -> void = 0;

  protected:
    [[nodiscard]] auto is_node_transform_active() const noexcept -> bool {
        return node_transform != nullptr;
    }

  public:
    auto
    set_draw_callback(const std::function<void(SceneNode&)>& callback) noexcept
        -> void;
    auto set_transform(std::shared_ptr<TransformationComponent> transformation)
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
