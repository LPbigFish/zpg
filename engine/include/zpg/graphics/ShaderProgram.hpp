#include "zpg/graphics/Shader.hpp"
#include <GL/gl.h>

namespace zpg::graphics {

class ShaderProgram {
    GLuint id;

  public:
    ShaderProgram() = delete;
    ShaderProgram(Shader vertex, Shader fragment);
    auto set_shader_program() -> bool;
};

} // namespace zpg::graphics
