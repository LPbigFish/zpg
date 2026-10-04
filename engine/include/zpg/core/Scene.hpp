#pragma once

#include "zpg/graphics/DrawableObject.hpp"
#include <memory>
#include <vector>

namespace zpg::core {

class Scene {
    std::vector<std::unique_ptr<graphics::DrawableObject>> game_objects;
    std::vector<std::shared_ptr<graphics::ShaderProgram>> shader_programs;

  public:
    Scene() = default;
    virtual ~Scene() = default;

    Scene(const Scene&) = delete;
    auto operator=(const Scene&) -> Scene& = delete;
    Scene(Scene&&) = default;
    auto operator=(Scene&&) -> Scene& = default;

    auto add_game_object(std::unique_ptr<graphics::DrawableObject> game_object)
        -> void {
        game_objects.push_back(std::move(game_object));
    }

    auto
    add_shader_program(std::shared_ptr<graphics::ShaderProgram> shader_program)
        -> void {
        shader_programs.push_back(std::move(shader_program));
    }

    virtual auto render() -> void;
};

} // namespace zpg::core
