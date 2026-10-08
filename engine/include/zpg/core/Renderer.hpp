#pragma once

#include "zpg/core/Scene.hpp"
#include <cstdint>

namespace zpg::core {

class Renderer {
  public:
    static void render(Scene& scene);
    static void set_viewport(int32_t width, int32_t height);
    static void setup_frame();
    static void setup_rendering();
};

} // namespace zpg::core
