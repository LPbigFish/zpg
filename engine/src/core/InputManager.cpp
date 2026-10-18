#include "zpg/core/InputManager.hpp"
#include <glm/ext/vector_float3.hpp>
#include <glm/geometric.hpp>

namespace zpg::core {

auto InputManager::subscribe(gfobserver::GlfwObserver& observer) -> void {
    observer.subscribe<events::KeyEvent>([this](const auto& event) {
        on_key(event);
    });
    observer.subscribe<events::ButtonEvent>([this](const auto& event) {
        on_button(event);
    });
    observer.subscribe<events::CursorEvent>([this](const auto& event) {
        on_cursor(event);
    });
    observer.subscribe<events::WindowFocusEvent>([this](const auto& event) {
        on_focus(event.focused);
    });
}

auto InputManager::on_key(const events::KeyEvent& event) -> void {
    if (event.key < 0 || event.key > GLFW_KEY_LAST) {
        return;
    }

    keys.at(event.key) = event.action != GLFW_RELEASE;
}

auto InputManager::on_button(const events::ButtonEvent& event) -> void {
    if (event.button != GLFW_MOUSE_BUTTON_RIGHT) {
        return;
    }

    looking = event.action == GLFW_PRESS;
    cursor_valid = false;
    mouse_delta = glm::vec2{0.f};
}

auto InputManager::on_cursor(const events::CursorEvent& event) -> void {
    const glm::dvec2 current{event.x, event.y};

    if (cursor_valid && looking) {
        const auto delta = current - cursor;
        mouse_delta += glm::vec2{
          static_cast<float>(delta.x), static_cast<float>(-delta.y)
        };
    }

    cursor = current;
    cursor_valid = true;
}

auto InputManager::on_focus(bool focused) -> void {
    if (focused) {
        return;
    }

    keys.fill(false);
    looking = false;
    cursor_valid = false;
    mouse_delta = glm::vec2{0.f};
}

auto InputManager::reset_cursor() -> void {
    cursor_valid = false;
    mouse_delta = glm::vec2{0.f};
}

auto InputManager::update(Camera& camera, float dt) -> void {
    auto forward = camera.front();
    forward.y = 0.f;
    forward = glm::normalize(forward);

    const auto right
        = glm::normalize(glm::cross(forward, glm::vec3{0.f, 1.f, 0.f}));

    glm::vec3 movement{0.f};
    if (keys[GLFW_KEY_W]) {
        movement += forward;
    }
    if (keys[GLFW_KEY_S]) {
        movement -= forward;
    }
    if (keys[GLFW_KEY_D]) {
        movement += right;
    }
    if (keys[GLFW_KEY_A]) {
        movement -= right;
    }

    if (glm::dot(movement, movement) > 0.f) {
        constexpr float SPEED = 4.f;
        camera.move(glm::normalize(movement) * SPEED * dt);
    }

    if (mouse_delta != glm::vec2{0.f}) {
        camera.look(mouse_delta.x, mouse_delta.y);
        mouse_delta = glm::vec2{0.f};
    }
}

} // namespace zpg::core
