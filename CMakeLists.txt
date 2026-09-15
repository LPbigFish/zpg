cmake_minimum_required(VERSION 3.30)

project(
  ZPGSpace
  VERSION 0.1.0
  LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_SCAN_FOR_MODULES OFF)

option(ZPG_ENABLE_SANITIZERS
       "Enable AddressSanitizer and UndefinedBehaviorSanitizer" OFF)
option(ZPG_DISABLE_LEAK_DETECTION_FOR_TESTS
       "Disable LeakSanitizer for tests running under a tracer or debugger" OFF)
option(ZPG_BUILD_TESTS "Build engine tests" ON)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")

find_package(OpenGL REQUIRED)
find_package(glfw3 CONFIG REQUIRED)
find_package(glm CONFIG REQUIRED)

add_subdirectory(third_party/glad)
include(cmake/ZpgTarget.cmake)

add_subdirectory(engine)
add_subdirectory(game)
add_subdirectory(sandbox)
if(ZPG_BUILD_TESTS)
  enable_testing()
  add_subdirectory(tests)
endif()
