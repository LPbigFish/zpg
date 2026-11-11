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
    glStencilMask(0xFF);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
}

void Renderer::setup_rendering() {
    glEnable(GL_DEPTH_TEST);
    setup_stencil();
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
}

void Renderer::setup_stencil() {
    glEnable(GL_STENCIL_TEST);
    glStencilMask(0xFF);
    glClearStencil(0);
    glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
}
} // namespace zpg::core
