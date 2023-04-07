cmake_minimum_required(VERSION 3.22)

project(star_knight)

set(CMAKE_CXX_STANDARD 17)

find_package(SDL2 REQUIRED)

MESSAGE(STATUS "SDL Version found: ${SDL2_VERSION}")

add_executable(${PROJECT_NAME}
	src/main.cpp
)

target_link_libraries(${PROJECT_NAME}
	SDL2
)
