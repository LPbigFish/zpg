#include "zpg/graphics/DrawableObject.hpp"
#include "zpg/graphics/Rotation.hpp"
#include "zpg/graphics/Scale.hpp"
#include "zpg/graphics/Translation.hpp"
#include <memory>
#include <utility>

namespace zpg::graphics {

DrawableObject::DrawableObject(
    Model model, std::shared_ptr<ShaderProgram> shader_program
): model{std::move(model)}, shader_program{std::move(shader_program)} {}

DrawableObject::DrawableObject(DrawableObject&& other) noexcept:
    model{std::move(other.model)},
    shader_program{std::move(other.shader_program)},
    _transformation{std::move(other._transformation)} {
    if (other.is_node_transform_active()) {
        this->set_transform(_transformation);
        other.set_transform(nullptr);
    }
}

[[nodiscard]] auto DrawableObject::get_program() const -> const ShaderProgram& {
    return *this->shader_program.get();
}

auto DrawableObject::add_transformation(
    std::unique_ptr<TransformationComponent> transformation
) -> DrawableObject& {
    if (_transformation->get_children().empty()
        && !is_node_transform_active()) {
        this->set_transform(_transformation);
    }
    _transformation->add(std::move(transformation));
    return *this;
}

auto DrawableObject::add_transformation_front(
    std::unique_ptr<TransformationComponent> transformation
) -> DrawableObject& {
    if (_transformation->get_children().empty()
        && !is_node_transform_active()) {
        this->set_transform(_transformation);
    }
    _transformation->add_front(std::move(transformation));
    return *this;
}

auto DrawableObject::get_transformation_composite() const
    -> const std::shared_ptr<TransformationComposite>& {
    return _transformation;
}

auto DrawableObject::render_in_world(const glm::mat4& mat) -> void {
    shader_program->set_shader_program();

    auto loc = shader_program->get_uniform_location("modelMatrix");
    shader_program->set_uniform(loc, mat);
    model.bind();
    glDrawArrays(GL_TRIANGLES, 0, model.get_count());
    Model::unbind();
    shader_program->unset_shader_program();
}

auto DrawableObject::draw() noexcept -> void {
    render_in_world(glm::mat4{1.f});
}
} // namespace zpg::graphics
