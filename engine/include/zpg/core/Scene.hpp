#pragma once

#include "zpg/graphics/DrawableObject.hpp"
#include "zpg/graphics/Group.hpp"
#include "zpg/graphics/SceneNode.hpp"
#include <memory>
#include <span>
#include <utility>

namespace zpg::core {

class Scene {
    graphics::Group root;

    friend class Renderer;
    virtual auto render() -> void;

  public:
    Scene() = default;
    virtual ~Scene() = default;

    Scene(const Scene&) = delete;
    auto operator=(const Scene&) -> Scene& = delete;
    Scene(Scene&&) = delete;
    auto operator=(Scene&&) -> Scene& = delete;

    template<graphics::VertexLayout Layout>
    auto add_object(
        std::span<const float> data,
        std::shared_ptr<graphics::ShaderProgram> shader_program
    ) -> graphics::DrawableObject& {
        return root.add_object<Layout>(data, std::move(shader_program));
    }

    auto add_node(std::unique_ptr<graphics::SceneNode> node)
        -> graphics::SceneNode& {
        return root.add(std::move(node));
    }

    auto add_group() -> graphics::Group& {
        return root.add_group();
    }

    template<std::derived_from<graphics::TransformationComponent> T>
    auto add_group(T transformation) -> graphics::Group& {
        return root.add_group(std::move(transformation));
    }
};

} // namespace zpg::core
