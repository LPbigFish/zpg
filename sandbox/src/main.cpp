#include "zpg/core/GlType.hpp"
#include <GL/gl.h>
#include <array>
#include <cstddef>
#include <glm/ext/scalar_constants.hpp>
#define GLFW_INCLUDE_NONE
#include <zpg/core/Application.hpp>
#include <zpg/graphics/ShaderProgram.hpp>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

#include "models/opengl_logo.h"
#include "models/suzi_flat.h"
#include "models/suzi_smooth.h"
#include "models/text.h"

#include <print>

namespace zg = zpg::graphics;

namespace zc = zpg::core;

constexpr auto RADIANTS = []() -> std::array<float, 360> {
    std::array<float, 360> res{};
    for (std::size_t i = 0; i < 360; i++) {
        res.at(i) = static_cast<float>(i) * (glm::pi<float>() / 180);
    }
    return res;
}();

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
    glBufferData(GL_ARRAY_BUFFER, sizeof(points), &points, GL_STATIC_DRAW);

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

    auto vs = zg::VertexShader::create("shaders/transformation.vert");
    auto fs = zg::FragmentShader::create("shaders/transformation.frag");

    if (!vs) {
        std::println(stderr, "{}", vs.error());
        return 1;
    }
    if (!fs) {
        std::println(stderr, "{}", fs.error());
        return 1;
    }

    // Create and link the shader program
    zg::ShaderProgram basic_shader_program{*vs, *fs};

    std::size_t angle = 0;

    auto workflow = [&]() -> void {
        basic_shader_program.set_shader_program();
        glBindVertexArray(vao1);
        GLint loc = basic_shader_program.get_uniform_location("fragmentColor");

        if (loc != -1) {
            basic_shader_program.set_uniform(loc, 1.f, 1.f, 0.5f);
        } else {
            std::println(stderr, "uniform location was not found!");
        }

        GLint loc2 = basic_shader_program.get_uniform_location("angle");
        if (loc != -1) {
            basic_shader_program.set_uniform(loc2, RADIANTS.at(angle % 360));
        } else {
            std::println(stderr, "uniform location was not found!");
        }

        angle += 1;
        if (angle >= 360) {
            angle = 0;
        }

        basic_shader_program.set_shader_program();
        glDrawArrays(GL_TRIANGLES, 0, static_cast<GLsizei>(sizeof(points) / 6));
        basic_shader_program.unset_shader_program();
    };

    app.run(workflow);
}
