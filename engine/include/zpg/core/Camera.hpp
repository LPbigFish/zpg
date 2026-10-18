#pragma once
#include "zpg/core/CameraObserver.hpp"
#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>
#include <memory>
#include <vector>

namespace zpg::core {
class Camera {
    glm::vec3 position{0.f, 1.f, 5.f};

    float yaw{-90.f};
    float pitch{0.f};

    float fov{60.f};
    float aspect{4.f / 3.f};

    glm::mat4 view_matrix{1};
    glm::mat4 projection_matrix{1};

    std::vector<std::weak_ptr<CameraObserver>> observers;

    auto update_view() -> void;
    auto update_projection() -> void;

  public:
    Camera();
    auto move(const glm::vec3& direction) -> void;

    auto set_position(const glm::vec3& value) -> void;

    auto look(float dx, float dy) -> void;

    auto resize(int width, int height) -> void;

    [[nodiscard]] auto front() const -> glm::vec3;

    [[nodiscard]] auto get_view() const -> const glm::mat4&;
    [[nodiscard]] auto get_projection() const -> const glm::mat4&;

    auto attach(const std::shared_ptr<CameraObserver>& observer) -> void;
    auto detach(const CameraObserver* observer) -> void;
    auto publish() -> void;
};
} // namespace zpg::core
