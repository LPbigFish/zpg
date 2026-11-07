#include "zpg/graphics/Skybox.hpp"
#include <utility>

namespace zpg::graphics {

Skybox::Skybox(
    std::shared_ptr<const Model> model,
    std::shared_ptr<ShaderProgram> shader_program,
    Texture&& texture
):
    model{std::move(model)}, program{std::move(shader_program)},
    texture{std::move(texture)} {}

auto Skybox::create(
    const std::shared_ptr<ShaderProgram>& shader_program,
    std::shared_ptr<const Model> model,
    std::span<const std::string, 6> face_paths
) -> std::expected<Skybox, std::string> {
    auto texture = Texture::create_cubemap(face_paths.data());
    if (!texture) {
        return std::unexpected{std::move(texture).error()};
    }

    return Skybox{std::move(model), shader_program, std::move(*texture)};
}

auto Skybox::render(const core::Camera& camera) const -> void {
    glDepthMask(GL_FALSE);

    program->set_shader_program();

    const glm::mat4 view = glm::mat4(glm::mat3(camera.get_view()));

    program->set_uniform(program->get_uniform_location("viewMatrix"), view);

    program->set_uniform(
        program->get_uniform_location("projectionMatrix"),
        camera.get_projection()
    );

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, texture.get_id());

    program->set_uniform(
        program->get_uniform_location("skyboxTexture"), GLint{0}
    );

    model->bind();
    glDrawArrays(GL_TRIANGLES, 0, model->get_count());

    Model::unbind();
    glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
    program->unset_shader_program();

    glDepthMask(GL_TRUE);
}
} // namespace zpg::graphics
