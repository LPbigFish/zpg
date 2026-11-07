#include "../SceneBuilder.hpp"
#include "zpg/graphics/Rotation.hpp"
#include "zpg/graphics/Scale.hpp"
#include "zpg/graphics/Texture.hpp"
#include "zpg/graphics/Translation.hpp"
#include <memory>
#include <print>

namespace {
[[nodiscard]] auto load_texture(const std::string& path)
    -> std::shared_ptr<zg::Texture> {
    auto texture = zg::Texture::create(path);
    if (!texture) {
        std::println(stderr, "Failed to load texture: {}", texture.error());
        return nullptr;
    }
    return std::make_shared<zg::Texture>(std::move(*texture));
}
} // namespace

auto SceneBuilder::create_eb408_scene(
    const std::shared_ptr<zg::ShaderProgram>& shader_program
) -> std::unique_ptr<zc::Scene> {
    auto scene = std::make_unique<zc::Scene>();

    const auto room_texture = load_texture("assets/textures/eb408/T_EB408.png");
    const auto chair_texture
        = load_texture("assets/textures/eb408/T_OrangeChair_BaseColor.png");
    const auto table_texture
        = load_texture("assets/textures/eb408/T_Table_BaseColor.png");
    const auto whiteboard_texture
        = load_texture("assets/textures/eb408/T_Whiteboard_BaseColor.png");
    const auto door_texture = load_texture("assets/textures/door.png");

    if (!room_texture || !chair_texture || !table_texture || !whiteboard_texture
        || !door_texture) {
        return scene;
    }

    auto& room = scene->add_group();

    room.add_transformation(zg::Translation{0, -0.55f, 0})
        .add_transformation(zg::Rotation{'x', -90});

    room.add_object(model(ModelId::Eb408Room), shader_program, room_texture);

    room.add_object(model(ModelId::OrangeChair), shader_program, chair_texture);

    room.add_object(model(ModelId::Table), shader_program, table_texture);

    room.add_object(
        model(ModelId::Whiteboard), shader_program, whiteboard_texture
    );

    auto& door_group = scene->add_group();

    door_group.add_transformation(zg::Translation{0.f, -0.55f, -2.f})
        .add_transformation(zg::Scale{-1.f, 1.f, 1.f});

    door_group
        .add_object(model(ModelId::DoorFrame), shader_program, door_texture)
        .add_transformation(zg::Rotation{'x', -90.f})
        .add_transformation(zg::Scale{1.f, -1.f, 1.f})
        .add_transformation(zg::Scale{0.01f})
        .add_transformation(zg::Translation{0.f, 0.f, 0.f});

    door_group.add_object(model(ModelId::Door), shader_program, door_texture)
        .add_transformation(zg::Rotation{'x', -90.f})
        .add_transformation(zg::Scale{0.01f, -0.01f, 0.01f});

    return scene;
}
