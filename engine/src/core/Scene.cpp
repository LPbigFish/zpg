#include "zpg/core/Scene.hpp"
#include <utility>

namespace zpg::core {

Scene::Scene():
    root{[this](const auto& program) -> void { camera.attach(program); }} {}

auto Scene::render() -> void {
    root.render(glm::mat4{1.f});
}

auto Scene::get_camera() noexcept -> Camera& {
    return camera;
}

auto Scene::activate() -> void {
    camera.publish();
}

auto Scene::observe_program(
    const std::shared_ptr<graphics::ShaderProgram>& program
) -> void {
    camera.attach(program);
}

auto Scene::set_update(std::function<void(float)> callback) -> void {
    update_callback = std::move(callback);
}

auto Scene::update(float dt) -> void {
    if (update_callback) {
        update_callback(dt);
    }
}

auto Scene::add_node(std::unique_ptr<graphics::SceneNode> node)
    -> graphics::SceneNode& {
    return root.add(std::move(node));
}

auto Scene::add_group() -> graphics::Group& {
    return root.add_group();
}
} // namespace zpg::core
