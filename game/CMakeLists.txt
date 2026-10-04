add_executable(zpg_game src/main.cpp src/Application.cpp)
target_compile_definitions(zpg_game PRIVATE GLFW_INCLUDE_NONE)
target_link_libraries(zpg_game PRIVATE ZPG::Engine glfw glad)
zpg_configure_target(zpg_game)
