#include "../SceneBuilder.hpp"
#include "models/uv_plain.hpp"
#include "zpg/graphics/Texture.hpp"
#include "zpg/graphics/VertexArray.hpp"

auto SceneBuilder::create_plain_scene(
    const std::shared_ptr<zg::ShaderProgram>& shader_program
) -> std::unique_ptr<zc::Scene> {
    auto scene = std::make_unique<zc::Scene>();

    auto texture = zg::Texture::create("assets/textures/wooden_fence.png");
    if (!texture) {
        std::println(stderr, "Failed to load texture: {}", texture.error());
        return scene;
    }

    auto doors = zg::Texture::create("assets/textures/doors.png");

    scene->add_object<zg::VertexLayout::PosNormalUv>(uv_plain, shader_program);

    scene->set_update(
        [shader_program,
         texture
         = std::make_shared<zg::Texture>(std::move(*texture))](float) -> void {
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, texture->get_id());
            shader_program->set_shader_program();
            shader_program->set_uniform(
                shader_program->get_uniform_location("textureUnitID"), 0
            );
            shader_program->unset_shader_program();
        }
    );
    return scene;
}
