#pragma once

#include "zpg/graphics/DrawableObject.hpp"
#include "zpg/graphics/SceneNode.hpp"
#include <concepts>
#include <memory>
#include <span>
#include <utility>
#include <vector>

namespace zpg::graphics {
class Group final: public SceneNode {
    std::vector<std::unique_ptr<SceneNode>> _children;

    auto render_in_world(const glm::mat4& mat) -> void override;

  public:
    auto add(std::unique_ptr<SceneNode> child) -> SceneNode&;

    template<std::derived_from<SceneNode> T>
    auto add(std::unique_ptr<T> child) -> T& {
        auto& result = *child;
        add(std::unique_ptr<SceneNode>{std::move(child)});
        return result;
    }

    template<VertexLayout Layout>
    auto add_object(
        std::span<const float> data,
        std::shared_ptr<ShaderProgram> shader_program
    ) -> DrawableObject& {
        return add(
            std::make_unique<DrawableObject>(
                Model::create<Layout>(data).value(), std::move(shader_program)
            )
        );
    }

    auto add_group() -> Group& {
        return add(std::make_unique<Group>());
    }

    template<std::derived_from<TransformationComponent> T>
    auto add_group(T transformation) -> Group& {
        auto& group = add_group();
        group.set_transform(std::move(transformation));
        return group;
    }
};
} // namespace zpg::graphics
