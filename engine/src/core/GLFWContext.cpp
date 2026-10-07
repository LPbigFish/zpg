#include "zpg/core/GLFWContext.hpp"
#include <GLFW/glfw3.h>
#include <stdexcept>

namespace zpg::core {

GLFWContext::GLFWContext() {
    if (!glfwInit()) {
        throw std::runtime_error{"Failed to initialize GLFW library"};
    }
}

GLFWContext::~GLFWContext() {
    glfwTerminate();
}

} // namespace zpg::core
