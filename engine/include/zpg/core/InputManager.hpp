#pragma once

#include "zpg/core/Camera.hpp"
#include "zpg/core/GFObserver.hpp"
#include <GLFW/glfw3.h>
#include <array>
#include <glm/ext/vector_double2.hpp>
#include <glm/ext/vector_float2.hpp>

namespace zpg::core {
namespace events = gfobserver::events;

struct InputManager {
  private:
    std::array<bool, GLFW_KEY_LAST + 1> keys{};

    bool looking{false};
    bool cursor_valid{false};

    glm::dvec2 cursor{0, 0};
    glm::vec2 mouse_delta{0.f};

  public:
    auto subscribe(gfobserver::GlfwObserver& observer) -> void;
    auto on_key(const events::KeyEvent& e) -> void;
    auto on_button(const events::ButtonEvent& e) -> void;
    auto on_cursor(const events::CursorEvent& e) -> void;
    auto on_focus(bool focused) -> void;
    auto reset_cursor() -> void;
    auto update(Camera& camera, float dt) -> void;
};
} // namespace zpg::core
