#include "zpg/core/InputManager.hpp"
#include <array>
#include <glad/gl.h>
#include <glm/ext/vector_float3.hpp>
#include <glm/geometric.hpp>
#include <print>
#include <utility>

namespace {
auto stencil_button(
    const glm::dvec2& cursor,
    glm::ivec2 window_size,
    glm::ivec2 framebuffer_size
) -> void {
    if (window_size.x <= 0 || window_size.y <= 0 || framebuffer_size.x <= 0
        || framebuffer_size.y <= 0 || cursor.x < 0 || cursor.x >= window_size.x
        || cursor.y < 0 || cursor.y >= window_size.y) {
        return;
    }

    const auto x
        = static_cast<GLint>(cursor.x * framebuffer_size.x / window_size.x);
    const auto y
        = framebuffer_size.y - 1
        - static_cast<GLint>(cursor.y * framebuffer_size.y / window_size.y);

    std::array<GLubyte, 4> color{};
    GLfloat depth{};
    GLuint index{};

    glReadBuffer(GL_BACK);
    glReadPixels(x, y, 1, 1, GL_RGBA, GL_UNSIGNED_BYTE, color.data());
    glReadPixels(x, y, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &depth);
    glReadPixels(x, y, 1, 1, GL_STENCIL_INDEX, GL_UNSIGNED_INT, &index);

    std::println(
        "Clicked on framebuffer pixel {}, {}, color {:02x}{:02x}{:02x}{:02x}, "
        "depth {}, "
        "stencil index {}",
        x,
        y,
        color[0],
        color[1],
        color[2],
        color[3],
        depth,
        index
    );
}
} // namespace

namespace zpg::core {

auto InputManager::subscribe(gfobserver::GlfwObserver& observer) -> void {
    observer.subscribe<events::KeyEvent>([this](const auto& event) -> auto {
        on_key(event);
    });
    observer.subscribe<events::ButtonEvent>([this](const auto& event) -> auto {
        on_button(event);
    });
    observer.subscribe<events::CursorEvent>([this](const auto& event) -> auto {
        on_cursor(event);
    });
    observer.subscribe<events::WindowFocusEvent>(
        [this](const auto& event) -> auto { on_focus(event.focused); }
    );
}

auto InputManager::on_key(const events::KeyEvent& event) -> void {
    if (event.key < 0 || event.key > GLFW_KEY_LAST) {
        return;
    }

    keys.at(event.key) = event.action != GLFW_RELEASE;
}

auto InputManager::on_button(const events::ButtonEvent& event) -> void {
    if (event.button == GLFW_MOUSE_BUTTON_LEFT) {
        if (event.action == GLFW_PRESS) {
            pending_stencil_click = cursor;
        }
        return;
    }

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
    pending_stencil_click.reset();
    cursor_valid = false;
    mouse_delta = glm::vec2{0.f};
}

auto InputManager::reset_cursor() -> void {
    cursor_valid = false;
    mouse_delta = glm::vec2{0.f};
    pending_stencil_click.reset();
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

auto InputManager::process_stencil_click(
    glm::ivec2 window_size, glm::ivec2 framebuffer_size
) -> void {
    if (auto click = std::exchange(pending_stencil_click, std::nullopt)) {
        stencil_button(*click, window_size, framebuffer_size);
    }
}

} // namespace zpg::core
