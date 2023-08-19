# Created on: 07/05/23.
# Author: DendyA

CMAKE_MINIMUM_REQUIRED(VERSION 3.22)

PROJECT(star_knight_models)

SET(CMAKE_CXX_STANDARD 17)

LIST(APPEND sk_models_lib_srcs
    sk_parser.cpp
    sk_mesh.cpp
    sk_mesh_instance.cpp
    sk_mesh_primitive.cpp
)

LIST(APPEND sk_models_lib_hdrs
    sk_parser.h
    sk_mesh.h
    sk_mesh_instance.h
    sk_mesh_primitive.h
)

# Make a models CMake library.
ADD_LIBRARY(${PROJECT_NAME}
    ${sk_models_lib_srcs}
    ${sk_models_lib_hdrs}
)

# TODO(DendyA): This will need to be not set for release builds in the future.
TARGET_COMPILE_DEFINITIONS(${PROJECT_NAME} PUBLIC
    BX_CONFIG_DEBUG
)

TARGET_INCLUDE_DIRECTORIES(${PROJECT_NAME} PUBLIC
    ${CMAKE_SOURCE_DIR}/src/defines # Includes the sk_global_defines.h header.

    ${CMAKE_SOURCE_DIR}/lib/bgfx_cmake/bx/include/
    ${CMAKE_SOURCE_DIR}/lib/bgfx_cmake/bgfx/include
)
