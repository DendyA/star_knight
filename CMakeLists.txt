# Created on: 26/03/23.
# Author: DendyA

CMAKE_MINIMUM_REQUIRED(VERSION 3.22)

PROJECT(star_knight)

SET(CMAKE_CXX_STANDARD 17)

if(PROJECT_SOURCE_DIR STREQUAL PROJECT_BINARY_DIR)
	MESSAGE(FATAL_ERROR "In-source builds not allowed. Please make a build directory (e.g. build) and run \"cmake ..\" from there.\n")
ENDIF()

# External libraries to build
ADD_SUBDIRECTORY(lib/SDL2)
ADD_SUBDIRECTORY(lib/bgfx_cmake)

# Engine components to build
ADD_SUBDIRECTORY(src/shaders)
ADD_SUBDIRECTORY(src/window_and_user)

ADD_EXECUTABLE(${PROJECT_NAME}
	src/main.cpp
)

TARGET_INCLUDE_DIRECTORIES(${PROJECT_NAME} PUBLIC
		${CMAKE_SOURCE_DIR}/lib/bgfx_cmake/bx/include/
)

TARGET_LINK_LIBRARIES(${PROJECT_NAME} PUBLIC
		SDL2
		bgfx
		star_knight_shaders
		star_knight_window_and_user
)
