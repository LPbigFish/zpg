add_library(glad INTERFACE)
add_library(stb_image INTERFACE)

target_include_directories(glad SYSTEM INTERFACE "${CMAKE_CURRENT_SOURCE_DIR}/include")

target_include_directories(
  stb_image SYSTEM INTERFACE "${CMAKE_CURRENT_SOURCE_DIR}/include/stb_image")
