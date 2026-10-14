file(GLOB_RECURSE ZPG_SANDBOX_SCENES CONFIGURE_DEPENDS
     "${CMAKE_CURRENT_SOURCE_DIR}/src/scenes/*.cpp")

add_executable(zpg_sandbox src/main.cpp src/Application.cpp
                           src/models/TextModel.cpp ${ZPG_SANDBOX_SCENES})

target_include_directories(zpg_sandbox PRIVATE src)
target_compile_definitions(zpg_sandbox PRIVATE GLFW_INCLUDE_NONE)
target_link_libraries(zpg_sandbox PRIVATE ZPG::Engine glfw glad)
zpg_configure_target(zpg_sandbox)
