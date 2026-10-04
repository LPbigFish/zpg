add_executable(zpg_sandbox src/main.cpp src/Application.cpp)
target_compile_definitions(zpg_sandbox PRIVATE GLFW_INCLUDE_NONE)
target_link_libraries(zpg_sandbox PRIVATE ZPG::Engine glfw glad)
zpg_configure_target(zpg_sandbox)
