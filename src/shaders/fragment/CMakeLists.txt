# Created on: 19/04/23.
# Author: DendyA

CMAKE_MINIMUM_REQUIRED(VERSION 3.22)

PROJECT(star_knight_shaders_fragment)

SET(CMAKE_CXX_STANDARD 17)

# ======================================= Define Paths ====================================

# *_path in the variable name implies that this is the path to an executable. No appending happens later!
SET(shader_compiler_path "${CMAKE_BINARY_DIR}/lib/bgfx_cmake/cmake/bgfx/shaderc")

# *_dir in the variable name implies that this is a directory that will be appended to later.
SET(compiled_shader_out_dir "${CMAKE_SOURCE_DIR}/compiled_shaders/fragment")
SET(base_shader_source_dir "${CMAKE_SOURCE_DIR}/src/shaders")
SET(shader_helper_include_dir "${CMAKE_SOURCE_DIR}/lib/bgfx_cmake/bgfx/src")

SET(varying_def_dir "${base_shader_source_dir}/utils")
SET(fragment_shader_source_dir "${base_shader_source_dir}/fragment")


# ======================================= Generate Output Dir (if it doesn't exist)  ================================
file(MAKE_DIRECTORY ${compiled_shader_out_dir})

# =========================================== Compile Fragment Shaders ============================

# Set up the list that will hold the names of all compiled fragment shaders.
# Used to define a DEPENDS list for the custom target.
LIST(APPEND sk_fragment_shaders_out)

# The name of all fragment shaders to be compiled. Do not include file extensions as part of the name.
# As more are created, add them here. One per line; preferably in alphabetical order.
LIST(APPEND sk_fragment_shaders
        fs_simple
)

FOREACH(fragment_shader IN LISTS sk_fragment_shaders)

    # Compile a given fragment shader.
    ADD_CUSTOM_COMMAND(
            # Compiled shader. Must match the list append below and the -o flag in the ARGS.
            OUTPUT ${compiled_shader_out_dir}/${fragment_shader}.bin
            COMMAND ${shader_compiler_path} # Path to the shaderc tool
            ARGS -f ${fragment_shader_source_dir}/${fragment_shader}.sc # Input shader file
            -o ${compiled_shader_out_dir}/${fragment_shader}.bin # Output compiled shader file.
            --platform linux # Platform to compile the shader for. TODO(DendyA): This will need to be platform-agnostic eventually.
            --type fragment # Type of shader to compile
            --verbose # Verbose output
            --varyingdef ${varying_def_dir}/varying.def.sc # Varying def path. TODO(DendyA): Is there only ever one varying def file?
            -i ${shader_helper_include_dir} # Input dir for any headers used by the shaders.
    )

    # Append the name and full path of the compiled shader to the output list.
    LIST(APPEND sk_fragment_shaders_out ${compiled_shader_out_dir}/${fragment_shader}.bin)
ENDFOREACH()

# Create a custom target that depends on all compiled fragment shaders.
ADD_CUSTOM_TARGET(compile_fragment_shaders ALL DEPENDS ${sk_fragment_shaders_out})