#pragma once

#include "zpg/core/Camera.hpp"
#include "zpg/graphics/DrawableObject.hpp"
#include "zpg/graphics/Group.hpp"
#include "zpg/graphics/SceneNode.hpp"
#include "zpg/graphics/Skybox.hpp"
#include <concepts>
#include <functional>
#include <memory>
#include <optional>
#include <utility>

namespace zpg::core {

class Scene {
    graphics::Group root;
    core::Camera camera;
    std::unique_ptr<graphics::Skybox> skybox;

    std::function<void(float)> update_callback;

    friend class Renderer;
    auto render() -> void;

  public:
    Scene();

    ~Scene() = default;

    Scene(const Scene&) = delete;
    auto operator=(const Scene&) -> Scene& = delete;
    Scene(Scene&&) = delete;
    auto operator=(Scene&&) -> Scene& = delete;

    [[nodiscard]] auto get_camera() noexcept -> Camera&;

    auto activate() -> void;

    auto
    observe_program(const std::shared_ptr<graphics::ShaderProgram>& program)
        -> void;

    auto set_update(std::function<void(float)> callback) -> void;

    auto update(float dt) -> void;

    auto add_object(
        std::shared_ptr<const graphics::Model> model,
        std::shared_ptr<graphics::ShaderProgram> shader_program,
        std::optional<std::shared_ptr<graphics::Texture>> texture = std::nullopt
    ) -> graphics::DrawableObject& {
        return root.add_object(
            std::move(model), std::move(shader_program), std::move(texture)
        );
    }

    auto add_node(std::unique_ptr<graphics::SceneNode> node)
        -> graphics::SceneNode&;

    auto set_skybox(std::unique_ptr<graphics::Skybox> skybox) -> void;

    auto add_group() -> graphics::Group&;

    template<std::derived_from<graphics::TransformationComponent> T>
    auto add_group(T transformation) -> graphics::Group& {
        return root.add_group(std::move(transformation));
    }
};

} // namespace zpg::core
