cmake_minimum_required(VERSION 3.10.0)
project(UCSG)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY bin/)
add_executable(UCSG src/main.cpp src/glad.c)

set(CMAKE_PREFIX_PATH "/mingw64")

find_package(GLFW3 REQUIRED)

target_link_libraries(UCSG glfw ${CMAKE_SOURCE_DIR}/lib/libbox2d.a)
target_include_directories(UCSG PRIVATE include/)
