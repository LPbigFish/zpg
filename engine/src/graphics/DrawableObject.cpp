#include "zpg/graphics/DrawableObject.hpp"

namespace zpg::graphics {

DrawableObject::DrawableObject(
    Model model, std::shared_ptr<ShaderProgram> shader_program
): model{std::move(model)}, shader_program{std::move(shader_program)} {}

DrawableObject::DrawableObject(DrawableObject&& other) noexcept:
    model{std::move(other.model)},
    shader_program{std::move(other.shader_program)} {}

[[nodiscard]] auto DrawableObject::get_program() const -> ShaderProgram& {
    return *this->shader_program.get();
}

auto DrawableObject::draw() noexcept -> void {
    shader_program->set_shader_program();
    model.bind();

    draw_callback(*this);
    glDrawArrays(GL_TRIANGLES, 0, model.get_count());
    model.unbind();
    shader_program->unset_shader_program();
}

auto DrawableObject::set_draw_callback(
    const std::function<void(DrawableObject&)>& callback
) noexcept -> void {
    draw_callback = callback;
}
} // namespace zpg::graphics
