#include <GL/gl.h>
#include <GLFW/glfw3.h>

#include <cstdlib>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

#include <memory>
#include <print>

namespace {

double dir = 1.0;

void error_callback(int /*error*/, const char* description) {
    fputs(description, stderr);
}

auto key_callback(
    GLFWwindow* window, int key, int scancode, int action, int mods
) -> void {
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, GL_TRUE);
    }
    if (key == GLFW_KEY_SPACE && action == GLFW_PRESS) {
        dir *= -1;
        std::println("space pressed, reversing rotation direction: {}", dir);
    }
    std::println("key_callback [{},{},{},{}]", key, scancode, action, mods);
}

auto window_focus_callback(GLFWwindow* /*window*/, int focused) -> void {
    std::println("window_focus_callback [{}]", focused);
}

auto window_iconify_callback(GLFWwindow* /*window*/, int iconified) -> void {
    std::println("window_iconify_callback [{}]", iconified);
}

auto window_size_callback(GLFWwindow* /*window*/, int width, int height)
    -> void {
    std::println("resize {}, {}", width, height);
    glViewport(0, 0, width, height);
}

auto cursor_callback(GLFWwindow* /*window*/, double x, double y) -> void {
    std::println("cursor_callback [{}, {}]", x, y);
}

auto button_callback(GLFWwindow* /*window*/, int button, int action, int mode)
    -> void {
    if (action == GLFW_PRESS) {
        std::println("button_callback [{},{},{}]", button, action, mode);
    }
}
}; // namespace

const glm::mat4 projection
    = glm::perspective(45.0f, 4.0f / 3.0f, 0.01f, 100.0f);

const glm::mat4 view = glm::lookAt(
    glm::vec3(10, 10, 10), // Camera is at (4,3,-3), in World Space
    glm::vec3(0, 0, 0),    // and looks at the origin
    glm::vec3(0, 1, 0)     // Head is up (set to 0,-1,0 to look upside-down)
);

// Model matrix : an identity matrix (model will be at the origin)
// const glm::mat4 model = glm::mat4(1.0f);

auto main() -> int {
    std::println("ZPG sandbox scaffold");

    glfwSetErrorCallback(error_callback);

    if (!glfwInit()) {
        exit(EXIT_FAILURE);
        return -1;
    }

    std::unique_ptr<GLFWwindow, decltype(&glfwDestroyWindow)> window{
      glfwCreateWindow(640, 480, "ZPG Sandbox", nullptr, nullptr),
      &glfwDestroyWindow
    };

    if (!window) {
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    glfwMakeContextCurrent(window.get());
    glfwSwapInterval(1);

    glfwSetKeyCallback(window.get(), key_callback);
    glfwSetWindowFocusCallback(window.get(), window_focus_callback);
    glfwSetWindowIconifyCallback(window.get(), window_iconify_callback);
    glfwSetWindowSizeCallback(window.get(), window_size_callback);
    glfwSetCursorPosCallback(window.get(), cursor_callback);
    glfwSetMouseButtonCallback(window.get(), button_callback);

    int width = 0;
    int height = 0;
    glfwGetFramebufferSize(window.get(), &width, &height);
    double ratio = static_cast<double>(width) / static_cast<double>(height);

    glViewport(0, 0, width, height);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-ratio, ratio, -1.0, 1.0, 1.0, -1.0);

    double current_angle = 0.0;
    double last_time = glfwGetTime();
    while (!glfwWindowShouldClose(window.get())) {
        glClear(GL_COLOR_BUFFER_BIT);

        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();

        auto time = glfwGetTime() - last_time;
        last_time = glfwGetTime();
        current_angle += dir * time * 50.0;
        glRotated(current_angle, 0.0, 0.0, 1.0);

        glTranslatef(0.6f, 0.6f, 0.f);

        glBegin(GL_TRIANGLES);

        {
            glColor3f(1.f, 0.f, 0.f);
            glVertex3f(-0.6f, -0.6f, 0.f);
            glColor3f(0.f, 1.f, 0.f);
            glVertex3f(0.6f, -0.6f, 0.f);
            glColor3f(0.f, 0.f, 1.f);
            glVertex3f(0.6f, 0.6f, 0.f);
        }
        {
            glColor3f(1.f, 0.f, 0.f);
            glVertex3f(-0.6f, -0.6f, 0.f);
            glColor3f(0.f, 0.f, 1.f);
            glVertex3f(0.6f, 0.6f, 0.f);
            glColor3f(0.f, 1.f, 1.f);
            glVertex3f(-0.6f, 0.6f, 0.f);
        }

        glEnd();
        glfwSwapBuffers(window.get());
        glfwPollEvents();
    }

    window.reset();
    glfwTerminate();
    exit(EXIT_SUCCESS);
}
