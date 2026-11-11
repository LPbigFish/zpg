#pragma once

#include "zpg/core/Camera.hpp"
#include "zpg/core/GFObserver.hpp"
#include <GLFW/glfw3.h>
#include <array>
#include <glm/ext/vector_double2.hpp>
#include <glm/ext/vector_float2.hpp>
#include <glm/ext/vector_int2.hpp>
#include <optional>

namespace zpg::core {
namespace events = gfobserver::events;

struct InputManager {
  private:
    std::array<bool, GLFW_KEY_LAST + 1> keys{};

    bool looking{false};
    bool cursor_valid{false};

    glm::dvec2 cursor{0, 0};
    glm::vec2 mouse_delta{0.f};
    std::optional<glm::dvec2> pending_stencil_click;

  public:
    auto subscribe(gfobserver::GlfwObserver& observer) -> void;
    auto on_key(const events::KeyEvent& e) -> void;
    auto on_button(const events::ButtonEvent& e) -> void;
    auto on_cursor(const events::CursorEvent& e) -> void;
    auto on_focus(bool focused) -> void;
    auto reset_cursor() -> void;
    auto update(Camera& camera, float dt) -> void;

    auto
    process_stencil_click(glm::ivec2 window_size, glm::ivec2 framebuffer_size)
        -> void;
};
} // namespace zpg::core
