#include "zpg/core/GlType.hpp"
#include <array>
#define GLFW_INCLUDE_NONE
#include <zpg/core/Application.hpp>
#include <zpg/graphics/ShaderProgram.hpp>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

#include "models/suzi_flat.h"
#include "models/suzi_smooth.h"

#include <print>

namespace zg = zpg::graphics;

namespace zc = zpg::core;

const std::array<float, 36> points{
  // First triangle
  -0.5f,
  0.5f,
  0.0f,
  1.0f,
  0.0f,
  0.0f,
  0.5f,
  0.5f,
  0.0f,
  0.0f,
  1.0f,
  0.0f,
  -0.5f,
  -0.5f,
  0.0f,
  0.0f,
  0.0f,
  1.0f,
  // Second triangle
  0.5f,
  0.5f,
  0.0f,
  0.0f,
  1.0f,
  0.0f,
  -0.5f,
  -0.5f,
  0.0f,
  0.0f,
  0.0f,
  1.0f,
  0.5f,
  -0.5f,
  0.0f,
  1.0f,
  1.0f,
  0.0f
};

const glm::mat4 projection
    = glm::perspective(45.0f, 4.0f / 3.0f, 0.01f, 100.0f);

const glm::mat4 view = glm::lookAt(
    glm::vec3(10, 10, 10), // Camera is at (4,3,-3), in World Space
    glm::vec3(0, 0, 0),    // and looks at the origin
    glm::vec3(0, 1, 0)     // Head is up (set to 0,-1,0 to look upside-down)
);

auto main() -> int {
    std::println("ZPG sandbox scaffold");

    zc::Application app{};
    app.init();

    // vertex buffer object (vbo1)
    GLuint vbo1 = 0;
    glGenBuffers(1, &vbo1); // generate the vbo1
    glBindBuffer(GL_ARRAY_BUFFER, vbo1);
    glBufferData(GL_ARRAY_BUFFER, sizeof(suziFlat), &suziFlat, GL_STATIC_DRAW);

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

    // vertex buffer object (vbo2)
    GLuint vbo2 = 0;
    glGenBuffers(1, &vbo2); // generate the vbo1
    glBindBuffer(GL_ARRAY_BUFFER, vbo2);
    glBufferData(
        GL_ARRAY_BUFFER, sizeof(suziSmooth), &suziSmooth, GL_STATIC_DRAW
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

    auto basic_vertex_shader = zg::VertexShader::create("shaders/basic.vert");
    auto basic_fragment_shader
        = zg::FragmentShader::create("shaders/basic.frag");

    if (!basic_vertex_shader) {
        std::println(stderr, "{}", basic_vertex_shader.error());
        return 1;
    }
    if (!basic_fragment_shader) {
        std::println(stderr, "{}", basic_fragment_shader.error());
        return 1;
    }

    auto special_vertex_shader
        = zg::VertexShader::create("shaders/special.vert");
    auto special_fragment_shader
        = zg::FragmentShader::create("shaders/special.frag");

    if (!special_vertex_shader) {
        std::println(stderr, "{}", special_vertex_shader.error());
        return 1;
    }
    if (!special_fragment_shader) {
        std::println(stderr, "{}", special_fragment_shader.error());
        return 1;
    }

    // Create and link the shader program
    zg::ShaderProgram basic_shader_program{
      basic_vertex_shader.value(), basic_fragment_shader.value()
    };

    zg::ShaderProgram special_shader_program{
      special_vertex_shader.value(), special_fragment_shader.value()
    };

    auto workflow = [&]() -> void {
        basic_shader_program.set_shader_program();
        glBindVertexArray(vao1);

        // Draw the six points as two triangles forming a rectangle.
        glDrawArrays(
            GL_TRIANGLES, 0, static_cast<GLsizei>(sizeof(suziFlat) / 6)
        );

        special_shader_program.set_shader_program();
        glBindVertexArray(vao2);

        glDrawArrays(
            GL_TRIANGLES, 0, static_cast<GLsizei>(sizeof(suziSmooth) / 6)
        );
    };

    app.run(workflow);
}
