#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#define GLAD_GL_IMPLEMENTATION
#include <glad/gl.h>

#include <array>
#include <cstdlib>
#include <fstream>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

#include "models/suzi_flat.h"
#include "models/suzi_smooth.h"

#include <memory>
#include <print>
#include <string>

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

auto create_shader_from_file(GLenum shaderType, const char* shaderFile)
    -> GLuint {
    // Creates an empty shader
    GLuint shader_id = glCreateShader(shaderType);

    if (shader_id == 0) {
        std::println("Unable to create shader");
        exit(EXIT_FAILURE);
    }

    // Loading the contents of a file into a variable
    std::ifstream file(shaderFile);
    if (!file.is_open()) {
        std::println("Unable to open file {}", shaderFile);
        glDeleteShader(shader_id);
        exit(-1);
    }
    std::string shader_code{
      (std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>()
    };

    // Set the shader source code
    const char* source = shader_code.c_str();
    glShaderSource(shader_id, 1, &source, nullptr);

    // Compile the shader source code
    glCompileShader(shader_id);

    // Check specialization/compilation status
    GLint success{};
    glGetShaderiv(shader_id, GL_COMPILE_STATUS, &success);
    if (!success) {
        std::array<char, 1024> info_log{};
        glGetShaderInfoLog(
            shader_id, sizeof(info_log), nullptr, info_log.data()
        );
        std::println("Shader failed: {}", info_log);
        glDeleteShader(shader_id);
        exit(1);
    }
    return shader_id;
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
      glfwCreateWindow(640 * 2, 480 * 2, "ZPG Sandbox", nullptr, nullptr),
      &glfwDestroyWindow
    };

    if (!window) {
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    glfwMakeContextCurrent(window.get());
    glfwSwapInterval(1);

    if (!gladLoadGL(reinterpret_cast<GLADloadfunc>(glfwGetProcAddress))) {
        std::println("GLAD initialization failed");
        return -1;
    }

    {
        std::println(
            "OpenGL Version: {}",
            reinterpret_cast<const char*>(glGetString(GL_VERSION))
        );
        std::println(
            "Vendor {}", reinterpret_cast<const char*>(glGetString(GL_VENDOR))
        );
        std::println(
            "Renderer {}",
            reinterpret_cast<const char*>(glGetString(GL_RENDERER))
        );
        std::println(
            "GLSL {}",
            reinterpret_cast<const char*>(
                glGetString(GL_SHADING_LANGUAGE_VERSION)
            )
        );
        int major{};
        int minor{};
        int revision{};
        glfwGetVersion(&major, &minor, &revision);
        std::println("Using GLFW {}.{}.{}", major, minor, revision);
    }

    int width = 0;
    int height = 0;
    glfwGetFramebufferSize(window.get(), &width, &height);

    glViewport(0, 0, width, height);

    // vertex buffer object (vbo1)
    GLuint vbo1 = 0;
    glGenBuffers(1, &vbo1); // generate the vbo1
    glBindBuffer(GL_ARRAY_BUFFER, vbo1);
    glBufferData(GL_ARRAY_BUFFER, sizeof(suziFlat), suziFlat, GL_STATIC_DRAW);

    // vertex buffer object (vbo2)
    GLuint vbo2 = 0;
    glGenBuffers(1, &vbo2); // generate the vbo1
    glBindBuffer(GL_ARRAY_BUFFER, vbo2);
    glBufferData(
        GL_ARRAY_BUFFER, sizeof(suziSmooth), suziSmooth, GL_STATIC_DRAW
    );

    // Vertex Array Object (vao1)
    GLuint vao1 = 0;
    glGenVertexArrays(1, &vao1);  // generate the vao1
    glBindVertexArray(vao1);      // bind the vao1
    glEnableVertexAttribArray(0); // enable vertex attributes
    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, vbo1);
    // index, number of components, data type, normalized, vertex stride, offset
    glVertexAttribPointer(
        0,
        3,
        GL_FLOAT,
        GL_FALSE,
        6 * sizeof(float),
        reinterpret_cast<GLvoid*>(0)
    );
    glVertexAttribPointer(
        1,
        3,
        GL_FLOAT,
        GL_FALSE,
        6 * sizeof(float),
        reinterpret_cast<GLvoid*>(3 * sizeof(float))
    );

    GLuint vao2 = 0;
    glGenVertexArrays(1, &vao2);  // generate the vao1
    glBindVertexArray(vao2);      // bind the vao1
    glEnableVertexAttribArray(0); // enable vertex attributes
    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, vbo2);
    // index, number of components, data type, normalized, vertex stride, offset
    glVertexAttribPointer(
        0,
        3,
        GL_FLOAT,
        GL_FALSE,
        6 * sizeof(float),
        reinterpret_cast<GLvoid*>(0)
    );
    glVertexAttribPointer(
        1,
        3,
        GL_FLOAT,
        GL_FALSE,
        6 * sizeof(float),
        reinterpret_cast<GLvoid*>(3 * sizeof(float))
    );

    GLuint basic_vertex_shader
        = create_shader_from_file(GL_VERTEX_SHADER, "shaders/basic.vert");
    GLuint basic_fragment_shader
        = create_shader_from_file(GL_FRAGMENT_SHADER, "shaders/basic.frag");
    GLuint special_vertex_shader
        = create_shader_from_file(GL_VERTEX_SHADER, "shaders/special.vert");
    GLuint special_fragment_shader
        = create_shader_from_file(GL_FRAGMENT_SHADER, "shaders/special.frag");

    // Create and link the shader program
    GLuint basic_shader_program = glCreateProgram();
    glAttachShader(basic_shader_program, basic_fragment_shader);
    glAttachShader(basic_shader_program, basic_vertex_shader);
    glLinkProgram(basic_shader_program);

    GLuint special_shader_program = glCreateProgram();
    glAttachShader(special_shader_program, special_fragment_shader);
    glAttachShader(special_shader_program, special_vertex_shader);
    glLinkProgram(special_shader_program);

    glEnable(GL_DEPTH_TEST);
    while (!glfwWindowShouldClose(window.get())) {
        // Clear color and depth buffer
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glUseProgram(basic_shader_program);
        glBindVertexArray(vao1);

        // Draw a triangles
        glDrawArrays(GL_TRIANGLES, 0, sizeof(suziFlat)); // mode,first,count

        glUseProgram(special_shader_program);
        glBindVertexArray(vao2);

        glDrawArrays(GL_TRIANGLES, 0, sizeof(suziSmooth));

        glfwSwapBuffers(window.get());
        glfwPollEvents();
    }

    window.reset();
    glfwTerminate();
    exit(EXIT_SUCCESS);
}
