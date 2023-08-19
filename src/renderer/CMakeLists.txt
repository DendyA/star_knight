# Created on: 26/04/23.
# Author: DendyA

CMAKE_MINIMUM_REQUIRED(VERSION 3.22)

PROJECT(star_knight_renderer)

SET(CMAKE_CXX_STANDARD 17)

# Append the shader manager class source file.
LIST(APPEND sk_renderer_lib_srcs
    initializer.cpp
    transformation_manager.cpp
)

LIST(APPEND sk_renderer_lib_hdrs
    initializer.h
    transformation_manager.cpp
)

# Make a shader CMake library.
ADD_LIBRARY(${PROJECT_NAME}
    ${sk_renderer_lib_srcs}
    ${sk_renderer_lib_hdrs}
)

# TODO(DendyA): This will need to be not set for release builds in the future.
TARGET_COMPILE_DEFINITIONS(${PROJECT_NAME} PUBLIC
    BX_CONFIG_DEBUG
)

TARGET_INCLUDE_DIRECTORIES(${PROJECT_NAME} PUBLIC
    ${CMAKE_SOURCE_DIR}/src/defines # Includes the sk_global_defines.h header.

    ${CMAKE_SOURCE_DIR}/lib/bgfx_cmake/bx/include/
    ${CMAKE_SOURCE_DIR}/lib/bgfx_cmake/bgfx/include
    ${CMAKE_BINARY_DIR}/lib/SDL2/include
    ${CMAKE_BINARY_DIR}/lib/SDL2/include-config-debug # TODO(DendyA): This will probably need to be changed to a release version in the future.
)
