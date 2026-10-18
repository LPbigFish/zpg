#pragma once

#include "zpg/graphics/Transformation.hpp"
#include "zpg/graphics/TransformationComponent.hpp"
#include "zpg/graphics/TransformationComposite.hpp"
#include <concepts>
#include <functional>
#include <glm/ext/matrix_float4x4.hpp>
#include <memory>
#include <utility>

namespace zpg::graphics {

class SceneNode: public Transformation {
    std::shared_ptr<TransformationComponent> node_transform{nullptr};
    std::shared_ptr<TransformationComposite> _transformation
        = std::make_shared<TransformationComposite>();

    std::function<void(SceneNode&)> draw_callback;

    virtual auto render_in_world(const glm::mat4& mat) -> void = 0;

  protected:
    [[nodiscard]] auto is_node_transform_active() const noexcept -> bool;

    auto move_transform_from(SceneNode& other) noexcept -> void;

  public:
    auto
    set_draw_callback(const std::function<void(SceneNode&)>& callback) noexcept
        -> void;
    auto set_transform(std::shared_ptr<TransformationComponent> transformation)
        -> SceneNode&;

    template<std::derived_from<TransformationComponent> T>
    auto set_transform(T transformation) -> SceneNode& {
        return set_transform(std::make_shared<T>(std::move(transformation)));
    }

    auto
    add_transformation(std::unique_ptr<TransformationComponent> transformation)
        -> SceneNode&;

    template<std::derived_from<TransformationComponent> T>
    auto add_transformation(T component) -> SceneNode& {
        return add_transformation(std::make_unique<T>(std::move(component)));
    }

    auto add_transformation_front(
        std::unique_ptr<TransformationComponent> transformation
    ) -> SceneNode&;

    template<std::derived_from<TransformationComponent> T>
    auto add_transformation_front(T component) -> SceneNode& {
        return add_transformation_front(
            std::make_unique<T>(std::move(component))
        );
    }

    [[nodiscard]] auto get_transformation_composite() const
        -> const std::shared_ptr<TransformationComposite>&;

    auto render(const glm::mat4& parent_matrix) -> void;
    SceneNode();
    SceneNode(const SceneNode&) = delete;
    auto operator=(const SceneNode&) -> SceneNode& = delete;
    SceneNode(SceneNode&&) = delete;
    auto operator=(SceneNode&&) -> SceneNode& = delete;
    virtual ~SceneNode() = default;
};
} // namespace zpg::graphics
