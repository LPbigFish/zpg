#pragma once

#include "zpg/graphics/DrawableObject.hpp"
#include "zpg/graphics/SceneNode.hpp"
#include <concepts>
#include <functional>
#include <memory>
#include <optional>
#include <utility>
#include <vector>

namespace zpg::graphics {
class Group final: public SceneNode {
    using ProgramObserver
        = std::function<void(const std::shared_ptr<ShaderProgram>&)>;

    std::vector<std::unique_ptr<SceneNode>> _children;
    ProgramObserver observer;

    auto render_in_world(const glm::mat4& mat) -> void override;

  public:
    explicit Group(ProgramObserver program_observer = {});

    auto add(std::unique_ptr<SceneNode> child) -> SceneNode&;

    template<std::derived_from<SceneNode> T>
    auto add(std::unique_ptr<T> child) -> T& {
        auto& result = *child;
        add(std::unique_ptr<SceneNode>{std::move(child)});
        return result;
    }

    auto add_object(
        std::shared_ptr<const Model> model,
        std::shared_ptr<ShaderProgram> shader_program,
        std::optional<std::shared_ptr<Texture>> texture = std::nullopt
    ) -> DrawableObject& {
        if (observer) {
            observer(shader_program);
        }

        return add(
            std::make_unique<DrawableObject>(
                std::move(model), std::move(shader_program), std::move(texture)
            )
        );
    }

    auto add_group() -> Group&;

    template<std::derived_from<TransformationComponent> T>
    auto add_group(T transformation) -> Group& {
        auto& group = add_group();
        group.set_transform(std::move(transformation));
        return group;
    }
};
} // namespace zpg::graphics
