# Generated GLAD sources are intentionally not checked in yet.
# Add glad/src/glad.c and glad/include when generation is complete.
add_library(glad STATIC src/gl.c)

target_include_directories(glad PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include")

set_target_properties(glad PROPERTIES
      C_STANDARD 99
      C_STANDARD_REQUIRED ON
  )
