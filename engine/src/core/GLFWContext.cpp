#include "zpg/core/GLFWContext.hpp"
#include <GLFW/glfw3.h>
#include <print>
#include <stdexcept>

namespace {
auto glfw_error_callback(int error, const char* description) -> void {
    std::println(stderr, "GLFW error [{}]: {}", error, description);
}
} // namespace

namespace zpg::core {

GLFWContext::GLFWContext() {
    glfwSetErrorCallback(glfw_error_callback);
    if (!glfwInit()) {
        throw std::runtime_error{"Failed to initialize GLFW library"};
    }
}

GLFWContext::~GLFWContext() {
    glfwTerminate();
}

} // namespace zpg::core
