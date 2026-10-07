#pragma once

namespace zpg::core {
class GLFWContext {
  public:
    GLFWContext();
    ~GLFWContext();

    GLFWContext(const GLFWContext&) = delete;
    auto operator=(const GLFWContext&) -> GLFWContext& = delete;
    GLFWContext(GLFWContext&&) = delete;
    auto operator=(GLFWContext&&) -> GLFWContext& = delete;
};
} // namespace zpg::core
