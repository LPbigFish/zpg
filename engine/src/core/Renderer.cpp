#include "zpg/core/Renderer.hpp"
#include "zpg/graphics/VertexBuffer.hpp"

namespace zpg::core {

void Renderer::render(Scene& scene) {
    scene.render();
}

void Renderer::set_viewport(int32_t width, int32_t height) {
    glViewport(0, 0, width, height);
}

void Renderer::setup_frame() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Renderer::setup_rendering() {
    glEnable(GL_DEPTH_TEST);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
}
} // namespace zpg::core
