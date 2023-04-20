CMAKE_MINIMUM_REQUIRED(VERSION 3.22)

PROJECT(star_knight_shaders)

SET(CMAKE_CXX_STANDARD 17)

# Add the different shader types as subdirectories
ADD_SUBDIRECTORY(fragment)
ADD_SUBDIRECTORY(vertex)

# Append the shader manager class source file.
LIST(APPEND sk_shader_lib
    shader_manager.cpp
)

# Make a shader CMake library.
ADD_LIBRARY(${PROJECT_NAME}
    ${sk_shader_lib}
)

# Add the two custom targets for compiling the shaders.
ADD_DEPENDENCIES(${PROJECT_NAME}
    compile_vertex_shaders
    compile_fragment_shaders
)