#pragma once

#include "zpg/graphics/Model.hpp"
#include "zpg/graphics/ShaderProgram.hpp"
#include <functional>
#include <glm/ext/vector_float3.hpp>
#include <glm/trigonometric.hpp>
#include <numbers>

constexpr auto RADIANTS = []() -> std::array<float, 360> {
    std::array<float, 360> res{};
    for (std::size_t i = 0; i < 360; i++) {
        res.at(i) = static_cast<float>(i) * (std::numbers::pi_v<float> / 180);
    }
    return res;
}();

namespace zpg::graphics {
class DrawableObject {
    Model model;
    std::shared_ptr<ShaderProgram> shader_program;
    std::function<void(DrawableObject&)> draw_callback{
      [](DrawableObject&) -> void {}
    };

  public:
    DrawableObject() = delete;

    explicit DrawableObject(
        Model model, std::shared_ptr<ShaderProgram> shader_program
    );

    auto set_draw_callback(
        const std::function<void(DrawableObject&)>& callback
    ) noexcept -> void;

    auto draw() noexcept -> void;

    [[nodiscard]] auto get_program() const -> ShaderProgram&;

    DrawableObject(DrawableObject&& other) noexcept;

    ~DrawableObject() = default;
    auto operator=(DrawableObject&& other) noexcept -> DrawableObject& = delete;
    DrawableObject(const DrawableObject& other) = delete;
    auto operator=(const DrawableObject& other) -> DrawableObject& = delete;
};
} // namespace zpg::graphics
