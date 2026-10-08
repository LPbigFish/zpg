#include "zpg/core/Scene.hpp"

namespace zpg::core {
auto Scene::render() -> void {
    root.render(glm::mat4{1.f});
}
} // namespace zpg::core
