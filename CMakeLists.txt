cmake_minimum_required(VERSION 3.13.0)
project(UCSG VERSION 1.0)

#Function to copy required dlls to binary folder
function (copy_dlls target)
    if (WIN32)
        add_custom_command (
            TARGET "${target}" POST_BUILD
            COMMAND "${CMAKE_COMMAND}" -E copy -t "$<TARGET_FILE_DIR:${target}>"
                    "$<TARGET_RUNTIME_DLLS:${target}>" USES_TERMINAL COMMAND_EXPAND_LISTS
        )
    endif ()
endfunction ()

message(STATUS "Compiler: ${CMAKE_CXX_COMPILER_ID} ${CMAKE_CXX_COMPILER_VERSION}")

set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

#mingw stuff
if(MINGW)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -static-libstdc++ -static-libgcc -static")
endif()

#Create sources + executable
set(APP_SRCS
    src/main.cpp 
    src/glad.c 
    src/input.cpp 
    src/renderer.cpp 
    src/sprite.cpp 
    src/area.cpp 
    src/debugger.cpp
)

set(IMGUI_SRCS 
    src/imgui/imgui.cpp 
    src/imgui/imgui_impl_glfw.cpp 
    src/imgui/imgui_tables.cpp 
    src/imgui/imgui_widgets.cpp
    src/imgui/imgui_draw.cpp 
    src/imgui/imgui_impl_opengl3.cpp
)

add_executable(UCSG ${APP_SRCS} ${IMGUI_SRCS})

#Packages + linking
find_package(glfw3 REQUIRED)
find_package(json-c CONFIG REQUIRED)
target_link_libraries(UCSG PRIVATE json-c::json-c glfw)
target_include_directories(UCSG PRIVATE include/ include/imgui/)

#Copy game resources
add_custom_command(TARGET UCSG POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_SOURCE_DIR}/resources "bin/resources"
    COMMENT "Copying resources to output directory"
)

copy_dlls(UCSG)

#Install rules
install(TARGETS UCSG RUNTIME DESTINATION ".")
install(DIRECTORY "resources/" DESTINATION "resources")

#TODO: Install dlls with cpack

#End of installer
set(CPACK_GENERATOR "ZIP")
include(CPack)