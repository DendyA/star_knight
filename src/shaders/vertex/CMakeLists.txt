# Created on: 19/04/23.
# Author: DendyA

CMAKE_MINIMUM_REQUIRED(VERSION 3.22)

PROJECT(star_knight_shaders_vertex)

SET(CMAKE_CXX_STANDARD 17)

# ======================================= Define Paths ====================================

# *_path in the variable name implies that this is the path to an executable. No appending later!
SET(shader_compiler_path "${CMAKE_BINARY_DIR}/lib/bgfx_cmake/cmake/bgfx/shaderc")

# *_dir in the variable name implies that this is a path that will be appended to later.
SET(compiled_shader_out_dir "${CMAKE_SOURCE_DIR}/compiled_shaders/vertex")
SET(base_shader_source_dir "${CMAKE_SOURCE_DIR}/src/shaders")
SET(shader_helper_include_dir "${CMAKE_SOURCE_DIR}/lib/bgfx_cmake/bgfx/src")

SET(varying_def_dir "${base_shader_source_dir}/utils")
SET(vertex_shader_source_dir "${base_shader_source_dir}/vertex")

# ======================================= Generate Output Dir (if it doesn't exist)  ================================
file(MAKE_DIRECTORY ${compiled_shader_out_dir})

# =========================================== Compile Vertex Shaders ============================

# Set up the list that will hold the names of all compiled vertex shaders.
# Used to define a DEPENDS list for the custom target.
LIST(APPEND sk_vertex_shaders_out)

# The name of all vertex shaders to be compiled. Do not include file extensions as part of the name.
# As more are created, add them here. One per line; preferably in alphabetical order.
LIST(APPEND sk_vertex_shaders
    vs_simple
)

FOREACH(vertex_shader IN LISTS sk_vertex_shaders)

    # Compile a given vertex shader.
    ADD_CUSTOM_COMMAND(
            # Compiled shader. Must match the LIST  below and the -o flag in the ARGS.
            OUTPUT ${compiled_shader_out_dir}/${vertex_shader}.bin
            COMMAND ${shader_compiler_path} # Path to the shaderc tool
            ARGS -f ${vertex_shader_source_dir}/${vertex_shader}.sc # Input shader file
            -o ${compiled_shader_out_dir}/${vertex_shader}.bin # Output compiled shader file.
            --platform linux # Platform to compile the shader for. TODO(DendyA): This will need to be platform-agnostic eventually.
            --type vertex # Type of shader to compile
            --verbose # Verbose output
            --varyingdef ${varying_def_dir}/varying.def.sc # Varying def path. TODO(DendyA): Is there only ever one varying def file?
            -i ${shader_helper_include_dir} # Input dir for any headers used by the shaders.
    )

    # Append the name and full path of the compiled shader to the output list.
    LIST(APPEND sk_vertex_shaders_out ${compiled_shader_out_dir}/${vertex_shader}.bin)
ENDFOREACH()

# Create a custom target that depends on all compiled vertex shaders.
ADD_CUSTOM_TARGET(compile_vertex_shaders ALL DEPENDS ${sk_vertex_shaders_out})