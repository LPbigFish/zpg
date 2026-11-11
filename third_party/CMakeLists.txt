add_library(glad INTERFACE)
add_library(stb_image INTERFACE)
add_library(tiny_obj INTERFACE)

target_include_directories(glad SYSTEM
                           INTERFACE "${CMAKE_CURRENT_SOURCE_DIR}/glad/include")

target_include_directories(
  stb_image SYSTEM INTERFACE "${CMAKE_CURRENT_SOURCE_DIR}/stb_image/include")

target_include_directories(
  tiny_obj SYSTEM INTERFACE "${CMAKE_CURRENT_SOURCE_DIR}/tiny_obj/include")
