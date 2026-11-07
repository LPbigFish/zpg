#pragma once

#include "zpg/graphics/Model.hpp"
#include <array>
#include <cstddef>
#include <cstdint>
#include <expected>
#include <memory>
#include <string>
#include <utility>

enum class ModelId : std::uint8_t {
    TriangleA,
    TriangleB,
    Text,
    SphereColor,
    SphereNormal,
    Plain,
    TreeColored,
    BushColor,
    BushColored,
    Clock,
    Slenderman,
    Sun,
    Earth,
    Moon,
    Satellite,
    UvPlain,
    Fiona,
    Shrek,
    Door,
    DoorFrame,
    Skybox,
    Eb408Room,
    OrangeChair,
    Table,
    Whiteboard,
    Count,
};

class ModelManager {
    static constexpr auto MODEL_COUNT
        = static_cast<std::size_t>(ModelId::Count);

    std::array<std::shared_ptr<const zpg::graphics::Model>, MODEL_COUNT> models;

    auto load(ModelId id, const zpg::graphics::MeshData& mesh)
        -> std::expected<void, std::string> {
        auto model = zpg::graphics::Model::create(mesh);
        if (!model) {
            return std::unexpected{std::move(model).error()};
        }

        models.at(static_cast<std::size_t>(id))
            = std::make_shared<const zpg::graphics::Model>(std::move(*model));
        return {};
    }

  public:
    static auto instance() -> ModelManager&;

    auto preload() -> std::expected<void, std::string>;

    [[nodiscard]] auto get(ModelId id) const
        -> const std::shared_ptr<const zpg::graphics::Model>&;

    auto clear() -> void;
};
