add_library(zpg_engine STATIC
    src/core/Application.cpp
    src/core/Window.cpp
    src/graphics/Shader.cpp
    src/graphics/Buffer.cpp
    src/graphics/VertexArray.cpp
    src/graphics/Texture.cpp
    src/graphics/Camera.cpp
)
add_library(ZPG::Engine ALIAS zpg_engine)

target_include_directories(zpg_engine PUBLIC include)
target_compile_definitions(zpg_engine PRIVATE GLFW_INCLUDE_NONE)
target_link_libraries(zpg_engine PUBLIC glm::glm PRIVATE OpenGL::GL glfw glad)
zpg_configure_target(zpg_engine)
