#pragma once

#include <glm/mat4x4.hpp>

namespace zpg::core {

class CameraObserver {
  public:
    CameraObserver() = default;
    virtual ~CameraObserver() = default;

    CameraObserver(const CameraObserver&) = default;
    auto operator=(const CameraObserver&) -> CameraObserver& = default;
    CameraObserver(CameraObserver&&) = default;
    auto operator=(CameraObserver&&) -> CameraObserver& = default;

    virtual auto
    on_camera_changed(const glm::mat4& view, const glm::mat4& projection)
        -> void
        = 0;
};

} // namespace zpg::core
