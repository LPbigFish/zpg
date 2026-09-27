#pragma once
#include <GLFW/glfw3.h>
#include <stdexcept>

namespace zpg::core {
class GLFWContext {
  public:
    GLFWContext() {
        if (!glfwInit()) {
            throw std::runtime_error{"Failed to initialize GLFW library"};
        }
    }

    ~GLFWContext() {
        glfwTerminate();
    }

    GLFWContext(const GLFWContext&) = delete;
    auto operator=(const GLFWContext&) -> GLFWContext& = delete;
    GLFWContext(GLFWContext&&) = delete;
    auto operator=(GLFWContext&&) -> GLFWContext& = delete;
};
} // namespace zpg::core
