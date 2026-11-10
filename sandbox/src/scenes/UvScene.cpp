#include "../SceneBuilder.hpp"
#include "ModelManager.hpp"
#include "zpg/graphics/Rotation.hpp"
#include "zpg/graphics/Scale.hpp"
#include "zpg/graphics/Skybox.hpp"
#include "zpg/graphics/Texture.hpp"
#include "zpg/graphics/Translation.hpp"
#include <array>
#include <memory>
#include <print>
#include <string>

auto SceneBuilder::create_plain_scene(
    const std::shared_ptr<zg::ShaderProgram>& shader_program,
    const std::shared_ptr<zg::ShaderProgram>& skybox_program,
    const std::shared_ptr<zg::ShaderProgram>& fixed_shader_program
) -> std::unique_ptr<zc::Scene> {
    auto scene = std::make_unique<zc::Scene>();

    const std::array<std::string, 6> skybox_faces{
      "assets/textures/skybox/posx.jpg",
      "assets/textures/skybox/negx.jpg",
      "assets/textures/skybox/posy.jpg",
      "assets/textures/skybox/negy.jpg",
      "assets/textures/skybox/posz.jpg",
      "assets/textures/skybox/negz.jpg"
    };
    auto skybox = zg::Skybox::create(
        skybox_program, model(ModelId::Skybox), skybox_faces
    );
    if (!skybox) {
        std::println(stderr, "Failed to create skybox: {}", skybox.error());
    } else {
        scene->set_skybox(std::make_unique<zg::Skybox>(std::move(*skybox)));
    }

    auto door = zg::Texture::create("assets/textures/door.png");
    if (!door) {
        std::println(stderr, "Failed to load texture: {}", door.error());
        return scene;
    }
    const auto door_texture = std::make_shared<zg::Texture>(std::move(*door));

    auto shrek = zg::Texture::create("assets/textures/shrek.png");
    if (!shrek) {
        std::println(stderr, "Failed to load texture: {}", shrek.error());
        return scene;
    }
    const auto shrek_texture = std::make_shared<zg::Texture>(std::move(*shrek));

    auto fiona = zg::Texture::create("assets/textures/fiona.png");
    if (!fiona) {
        std::println(stderr, "Failed to load texture: {}", fiona.error());
        return scene;
    }
    const auto fiona_texture = std::make_shared<zg::Texture>(std::move(*fiona));

    auto furry = zg::Texture::create("assets/textures/furry.png");
    if (!furry) {
        std::println(stderr, "Failed to load texture: {}", furry.error());
        return scene;
    }
    const auto furry_texture = std::make_shared<zg::Texture>(std::move(*furry));

    scene->add_object(model(ModelId::Shrek), shader_program, shrek_texture)
        .add_transformation(zg::Translation{{-2.f, 0.f, 0.f}});

    scene->add_object(model(ModelId::Fiona), shader_program, fiona_texture)
        .add_transformation(zg::Translation{{2.f, 0.f, 0.f}});

    // scene->add_object(model(ModelId::Furry), shader_program, furry_texture)
    //     .add_transformation(zg::Translation{{4.f, 1.f, 0.f}})
    //     .add_transformation(zg::Rotation{'y', -90.f})
    //     .add_transformation(zg::Rotation{'x', -90.f});

    auto& door_group = scene->add_group();

    door_group.add_transformation(zg::Translation{0.f, 0.f, -2.f});

    door_group
        .add_object(model(ModelId::DoorFrame), shader_program, door_texture)
        .add_transformation(zg::Rotation{'x', -90.f})
        .add_transformation(zg::Scale{1.f, -1.f, 1.f})
        .add_transformation(zg::Scale{0.01f})
        .add_transformation(zg::Translation{0.f, 0.f, 0.f});

    door_group.add_object(model(ModelId::Door), shader_program, door_texture)
        .add_transformation(zg::Translation{0.f, 0.f, -0.0f})
        .add_transformation(zg::Rotation{'x', -90.f})
        .add_transformation(zg::Scale{0.01f, -0.01f, 0.01f});

    add_signature(scene, fixed_shader_program);

    return scene;
}
