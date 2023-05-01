# Created on: 25/04/23.
# Author: DendyA

CMAKE_MINIMUM_REQUIRED(VERSION 3.22)

PROJECT(star_knight_window_and_user)

SET(CMAKE_CXX_STANDARD 17)

# Append the shader manager class source file.
LIST(APPEND sk_win_user_lib_srcs
        sk_window.cpp
)

LIST(APPEND sk_win_user_lib_hdrs
        sk_window.h
)

# Make a shader CMake library.
ADD_LIBRARY(${PROJECT_NAME}
        ${sk_win_user_lib_srcs}
        ${sk_win_user_lib_hdrs}
)

INCLUDE_DIRECTORIES(${PROJECT_NAME} PUBLIC
        ${CMAKE_BINARY_DIR}/lib/SDL2/include
        ${CMAKE_BINARY_DIR}/lib/SDL2/include-config-debug # TODO(DendyA): This will probably need to be changed to a release version in the future.
)