cmake_minimum_required(VERSION 3.10.0)
project(UCSG)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY bin/)
add_executable(UCSG src/main.cpp)

set(CMAKE_PREFIX_PATH "/mingw64")
find_package(GLFW3 REQUIRED)
find_package(OpenGL REQUIRED)

target_link_libraries(UCSG glfw)