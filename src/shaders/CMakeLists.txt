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

# TODO(DendyA): This will need to be not set for release builds in the future.
TARGET_COMPILE_DEFINITIONS(${PROJECT_NAME} PUBLIC
        BX_CONFIG_DEBUG
)

TARGET_INCLUDE_DIRECTORIES(${PROJECT_NAME} PUBLIC
    ${CMAKE_SOURCE_DIR}/src/models/

    ${CMAKE_SOURCE_DIR}/lib/bgfx_cmake/bx/include/
    ${CMAKE_SOURCE_DIR}/lib/bgfx_cmake/bgfx/include/
)

# Add the two custom targets for compiling the shaders.
ADD_DEPENDENCIES(${PROJECT_NAME}
    compile_vertex_shaders
    compile_fragment_shaders
)
