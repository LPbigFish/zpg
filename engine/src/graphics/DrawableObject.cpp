#include "zpg/graphics/DrawableObject.hpp"
#include <memory>
#include <optional>
#include <utility>

namespace zpg::graphics {

DrawableObject::DrawableObject(
    std::shared_ptr<const Model> model,
    std::shared_ptr<ShaderProgram> shader_program,
    std::optional<std::shared_ptr<Texture>> texture
):
    model{std::move(model)}, shader_program{std::move(shader_program)},
    texture{std::move(texture)} {}

DrawableObject::DrawableObject(DrawableObject&& other) noexcept:
    model{std::move(other.model)},
    shader_program{std::move(other.shader_program)},
    texture{std::move(other.texture)} {
    move_transform_from(other);
}

[[nodiscard]] auto DrawableObject::get_program() const -> const ShaderProgram& {
    return *this->shader_program.get();
}

auto DrawableObject::render_in_world(const glm::mat4& mat) -> void {
    shader_program->set_shader_program();

    if (texture) {
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, (*texture)->get_id());
        shader_program->set_uniform(
            shader_program->get_uniform_location("textureUnitID"), GLint{0}
        );
    }

    const glm::mat3 normal_matrix
        = glm::transpose(glm::inverse(glm::mat3(mat)));

    if (auto wn = shader_program->get_uniform_location("normalMatrix");
        wn != -1) {
        shader_program->set_uniform(wn, normal_matrix);
    }

    auto loc = shader_program->get_uniform_location("modelMatrix");
    shader_program->set_uniform(loc, mat);
    model->bind();
    glDrawArrays(GL_TRIANGLES, 0, model->get_count());
    Model::unbind();
    shader_program->unset_shader_program();
}

auto DrawableObject::draw() noexcept -> void {
    render_in_world(glm::mat4{1.f});
}
} // namespace zpg::graphics
