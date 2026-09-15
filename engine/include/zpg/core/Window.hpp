#pragma once

namespace zpg::core {

class Window {
  public:
    Window() = default;
    ~Window() = default;

    Window(const Window&) = delete;
    Window& operator=(const Window&) = delete;
    Window(Window&&) = delete;
    Window& operator=(Window&&) = delete;
};

} // namespace zpg::core
