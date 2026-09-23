add_library(glad INTERFACE)

target_include_directories(glad SYSTEM INTERFACE "${CMAKE_CURRENT_SOURCE_DIR}/include")
