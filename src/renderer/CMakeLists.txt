# Created on: 26/04/23.
# Author: DendyA

CMAKE_MINIMUM_REQUIRED(VERSION 3.22)

PROJECT(star_knight_renderer)

SET(CMAKE_CXX_STANDARD 17)

# Append the shader manager class source file.
LIST(APPEND sk_renderer_lib_srcs
    initializer.cpp
)

LIST(APPEND sk_renderer_lib_hdrs
    initializer.h
)

# Make a shader CMake library.
ADD_LIBRARY(${PROJECT_NAME}
    ${sk_renderer_lib_srcs}
    ${sk_renderer_lib_hdrs}
)

TARGET_INCLUDE_DIRECTORIES(${PROJECT_NAME} PUBLIC
        ${CMAKE_SOURCE_DIR}/lib/bgfx_cmake/bgfx/include/
        ${CMAKE_BINARY_DIR}/lib/SDL2/include
        ${CMAKE_BINARY_DIR}/lib/SDL2/include-config-debug # TODO(DendyA): This will probably need to be changed to a release version in the future.
)