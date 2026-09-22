#pragma once

namespace zpg::core {

class Window {
  public:
    Window() = default;
    ~Window() = default;

    Window(const Window&) = delete;
    auto operator=(const Window&) -> Window& = delete;
    Window(Window&&) = delete;
    auto operator=(Window&&) -> Window& = delete;
};

} // namespace zpg::core
