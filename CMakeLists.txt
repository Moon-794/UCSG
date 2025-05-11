cmake_minimum_required(VERSION 3.10.0)
project(UCSG)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY bin/)
set(IMGUI_SRCS src/imgui/imgui.cpp src/imgui/imgui_impl_glfw.cpp src/imgui/imgui_tables.cpp src/imgui/imgui_widgets.cpp src/imgui/imgui_draw.cpp src/imgui/imgui_impl_opengl3.cpp)
add_executable(UCSG src/main.cpp src/glad.c src/input.cpp src/renderer.cpp src/sprite.cpp src/debugger.cpp ${IMGUI_SRCS})

set(CMAKE_PREFIX_PATH "/mingw64")
set(CMAKE_BUILD_TYPE Debug)

find_package(GLFW3 REQUIRED)
find_package(json-c CONFIG REQUIRED)

target_link_libraries(UCSG PRIVATE json-c::json-c glfw Dwmapi)
target_include_directories(UCSG PRIVATE include/ include/imgui/)
