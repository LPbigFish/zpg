#include "zpg/core/Camera.hpp"
#include <algorithm>
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/trigonometric.hpp>

namespace zpg::core {

Camera::Camera() {
    update_view();
    update_projection();
}

auto Camera::update_view() -> void {
    this->view_matrix
        = glm::lookAt(position, position + front(), {0.f, 1.f, 0.f});

    this->projection_matrix
        = glm::perspective(glm::radians(fov), aspect, 0.1f, 150.f);
}

auto Camera::update_projection() -> void {
    this->view_matrix
        = glm::lookAt(position, position + front(), {0.f, 1.f, 0.f});

    this->projection_matrix
        = glm::perspective(glm::radians(fov), aspect, 0.1f, 150.f);
}

auto Camera::front() const -> glm::vec3 {
    const float y = glm::radians(yaw);
    const float p = glm::radians(pitch);

    return glm::normalize(
        glm::vec3{
          std::cos(y) * std::cos(p), std::sin(p), std::sin(y) * std::cos(p)
        }
    );
}

auto Camera::move(const glm::vec3& direction) -> void {
    position += direction;
    update_view();
    publish();
}

auto Camera::set_position(const glm::vec3& value) -> void {
    position = value;
    update_view();
    publish();
}

auto Camera::look(float dx, float dy) -> void {
    constexpr float SENSITIVITY = 0.1f;

    yaw += dx * SENSITIVITY;
    pitch = glm::clamp(pitch + (dy * SENSITIVITY), -89.f, 89.f);

    update_view();
    publish();
}

auto Camera::resize(int width, int height) -> void {
    if (width <= 0 || height <= 0) {
        return;
    }

    aspect = static_cast<float>(width) / static_cast<float>(height);

    update_projection();
    publish();
}

auto Camera::get_view() const -> const glm::mat4& {
    return view_matrix;
}

auto Camera::get_projection() const -> const glm::mat4& {
    return projection_matrix;
}

auto Camera::attach(const std::shared_ptr<CameraObserver>& observer) -> void {
    if (!observer) {
        return;
    }

    const auto found
        = std::ranges::any_of(observers, [&](const auto& weak) -> auto {
              return weak.lock() == observer;
          });

    if (found) {
        return;
    }

    observers.emplace_back(observer);

    observer->on_camera_changed(view_matrix, projection_matrix);
}

auto Camera::detach(const CameraObserver* observer) -> void {
    std::erase_if(observers, [&](const auto& weak) -> auto {
        const auto locked = weak.lock();
        return !locked || locked.get() == observer;
    });
}

auto Camera::publish() -> void {
    std::erase_if(observers, [](const auto& weak) -> auto {
        return weak.expired();
    });

    for (auto& weak : observers) {
        if (auto observer = weak.lock()) {
            observer->on_camera_changed(view_matrix, projection_matrix);
        }
    }
}
} // namespace zpg::core
