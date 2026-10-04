#include "zpg/core/Scene.hpp"

namespace zpg::core {
auto Scene::render() -> void {
    for (const auto& game_object : game_objects) {
        game_object->draw();
    }
}
} // namespace zpg::core
