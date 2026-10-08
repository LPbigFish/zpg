#include "zpg/graphics/DrawableObject.hpp"
#include "zpg/graphics/SceneNode.hpp"
#include <utility>

namespace zpg::graphics {

DrawableObject::DrawableObject(
    Model model, std::shared_ptr<ShaderProgram> shader_program
): model{std::move(model)}, shader_program{std::move(shader_program)} {}

DrawableObject::DrawableObject(DrawableObject&& other) noexcept:
    model{std::move(other.model)},
    shader_program{std::move(other.shader_program)} {}

[[nodiscard]] auto DrawableObject::get_program() const -> const ShaderProgram& {
    return *this->shader_program.get();
}

auto DrawableObject::render_in_world(const glm::mat4& mat) -> void {
    shader_program->set_shader_program();

    auto loc = shader_program->get_uniform_location("modelMatrix");
    shader_program->set_uniform(loc, mat);
    model.bind();
    glDrawArrays(GL_TRIANGLES, 0, model.get_count());
    model.unbind();
    shader_program->unset_shader_program();
}

auto DrawableObject::draw() noexcept -> void {
    render_in_world(glm::mat4{1.f});
}
} // namespace zpg::graphics
