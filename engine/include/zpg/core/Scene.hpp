#pragma once

#include "zpg/graphics/DrawableObject.hpp"
#include "zpg/graphics/Group.hpp"
#include "zpg/graphics/SceneNode.hpp"
#include <memory>
#include <utility>
#include <vector>

namespace zpg::core {

class Scene {
    graphics::Group root;
    std::vector<std::shared_ptr<graphics::ShaderProgram>> shader_programs;

  public:
    Scene() = default;
    virtual ~Scene() = default;

    Scene(const Scene&) = delete;
    auto operator=(const Scene&) -> Scene& = delete;
    Scene(Scene&&) = delete;
    auto operator=(Scene&&) -> Scene& = delete;

    auto add_game_object(std::unique_ptr<graphics::DrawableObject> game_object)
        -> void {
        root.add(std::move(game_object));
    }

    auto add_node(std::unique_ptr<graphics::SceneNode> node)
        -> graphics::SceneNode& {
        auto& n = *node;
        root.add(std::move(node));
        return n;
    }

    auto
    add_shader_program(std::shared_ptr<graphics::ShaderProgram> shader_program)
        -> void {
        shader_programs.push_back(std::move(shader_program));
    }

    virtual auto render() -> void;
};

} // namespace zpg::core
