# Created on: 18/04/23.
# Author: DendyA

CMAKE_MINIMUM_REQUIRED(VERSION 3.22)

PROJECT(star_knight_shaders)

SET(CMAKE_CXX_STANDARD 17)

# Add the different shader types as subdirectories
ADD_SUBDIRECTORY(fragment)
ADD_SUBDIRECTORY(vertex)

# Append the shader manager class source file.
LIST(APPEND sk_shader_lib_srcs
    shader_manager.cpp
)

LIST(APPEND sk_shader_lib_hdrs
    shader_manager.h
)

# Make a shader CMake library.
ADD_LIBRARY(${PROJECT_NAME}
    ${sk_shader_lib_srcs}
    ${sk_shader_lib_headers}
)

TARGET_INCLUDE_DIRECTORIES(${PROJECT_NAME} PUBLIC
    ${CMAKE_SOURCE_DIR}/lib/bgfx_cmake/bgfx/include/bgfx/
)

# Add the two custom targets for compiling the shaders.
ADD_DEPENDENCIES(${PROJECT_NAME}
    compile_vertex_shaders
    compile_fragment_shaders
)