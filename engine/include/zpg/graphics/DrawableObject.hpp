#pragma once

#include "zpg/graphics/Model.hpp"
#include "zpg/graphics/ShaderProgram.hpp"
#include <functional>
#include <glm/ext/vector_float3.hpp>
#include <glm/trigonometric.hpp>
#include <numbers>
#include <string_view>
#include <utility>

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
    glm::vec3 offset{0.0f, 0.0f, 0.0f};
    glm::vec3 scale{1.0f, 1.0f, 1.0f};
    std::int16_t angle{0};
    std::function<void(DrawableObject&)> draw_callback{
      [](DrawableObject&) -> void {}
    };

  public:
    DrawableObject() = delete;

    explicit DrawableObject(
        Model model, std::shared_ptr<ShaderProgram> shader_program
    ): model{std::move(model)}, shader_program{std::move(shader_program)} {}

    auto set_offset(const glm::vec3& new_offset) noexcept -> void {
        offset = new_offset;
    }

    auto set_scale(const glm::vec3& new_scale) noexcept -> void {
        scale = new_scale;
    }

    auto set_angle(std::int16_t new_angle) noexcept -> void {
        angle = static_cast<std::int16_t>(new_angle % 360);
    }

    auto set_draw_callback(
        const std::function<void(DrawableObject&)>& callback
    ) noexcept -> void {
        draw_callback = callback;
    }

    [[nodiscard]] auto get_offset() const noexcept -> glm::vec3 {
        return offset;
    }

    [[nodiscard]] auto get_scale() const noexcept -> glm::vec3 {
        return scale;
    }

    [[nodiscard]] auto get_angle() const noexcept -> std::int16_t {
        return angle;
    }

    auto draw() noexcept -> void {
        shader_program->set_shader_program();
        model.bind();

        draw_callback(*this);
        // NOLINTNEXTLINE
        this->set_uniform("offset", offset.x, offset.y, offset.z);
        // NOLINTNEXTLINE
        this->set_uniform("scale", scale.x, scale.y, scale.z);
        this->set_uniform("angle", RADIANTS.at(angle));
        glDrawArrays(GL_TRIANGLES, 0, model.get_count());
        model.unbind();
        shader_program->unset_shader_program();
    }

    DrawableObject(DrawableObject&& other) noexcept:
        model{std::move(other.model)},
        shader_program{std::move(other.shader_program)} {}

    ~DrawableObject() = default;
    auto operator=(DrawableObject&& other) noexcept -> DrawableObject& = delete;
    DrawableObject(const DrawableObject& other) = delete;
    auto operator=(const DrawableObject& other) -> DrawableObject& = delete;

    template<core::GlTypeConstrain... T>
    auto set_uniform(std::string_view name, T... values) -> void {
        static_assert(
            sizeof...(T) > 0 && sizeof...(T) <= 4,
            "At least one value must be provided, max 4 values are allowed"
        );
        const auto location = shader_program->get_uniform_location(name);

        shader_program->set_uniform(location, values...);
    }
};
} // namespace zpg::graphics
