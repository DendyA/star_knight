// Created on: 16/04/23
// Author: DendyA

#ifndef STAR_KNIGHT_SHADER_MANAGER_H
#define STAR_KNIGHT_SHADER_MANAGER_H

#include <vector>

#include "bgfx.h"

namespace star_knight
{
    /** ShaderManager class\n
     * The ShaderManager class is responsible for loading and initializing the vertex and fragment shaders,
     * creating the vertex and index buffers, and generating the shader programs from the vertex and fragment shaders.
     * All functions are static given that they don't rely on any member variables.
     * @todo Refactor the ShaderManager class once the shader subsystem interaction with the engine is better understood.
     */
    class ShaderManager
    {
        public:
            /** generateProgram\n
             * This program loads the two passed in shader files and creates the shader program from these and returns it to the
             * user by way of the passed-by-reference parameter.
             * It internally calls the loadShader() function which is responsible for reading the shader from disk.
             * @note It destroys the two loaded shader handles once they have been loaded into the shader program.
             * @param vertexShaderName The name of the vertex shader file on disk.
             * @param fragmentShaderName The name of the the fragment shader file on disk.
             * @param program The programHandle to save the created program to.
             * @return The result of running this function. True for success, false otherwise.
             */
            static bool generateProgram(const std::string& vertexShaderName, const std::string& fragmentShaderName, bgfx::ProgramHandle& program);

            /** initVertexBuffer\n
             * Creates a vertex buffer out of the supplied primitive and vertex layout struct.
             * @todo Remove the hard-coded primitive in this function and pass in when read-in from a file.
             * @return The handle to the init-ed vertex buffer.
             */
            static bgfx::VertexBufferHandle initVertexBuffer();

            /** initIndexBuffer\n
             * Creates an index buffer out of the supplied index list.
             * @todo Remove the hard-coded list of indexes in this function and pass in when primitive is read-in from a file.
             * @return The handle to the init-ed index buffer.
             */
            static bgfx::IndexBufferHandle initIndexBuffer();

        private:
            // Used to know which index of the COMPILED_SHADER_PATHS variable to use.
            // In other words, which type of shader is being read-in.
            enum ShaderManagerShaderTypes: uint32_t
            {
                kVertexShader = 0u,
                kFragmentShader
            };

            // List of the paths to the compiled shaders. This path is relative to the build folder.
            inline static const std::vector<std::string> COMPILED_SHADER_PATHS = {
                    "../compiled_shaders/vertex/",
                    "../compiled_shaders/fragment/"
            };

            /** loadShader\n
             * Reads-in a shader from disk and loads it into a ShaderHandle object.
             * @param shaderName The path to the shader file on disk.
             * @param typeIndex One of ShaderManagerShaderTypes for the type of shader pointed to by parameter shaderName.
             * @param handle The ShaderHandle to save the loaded shader to.
             * @return The result of running this function. True for success, false otherwise.
             */
            static bool loadShader(const std::string& shaderName, ShaderManagerShaderTypes typeIndex, bgfx::ShaderHandle& handle);
    };
} // star_knight

#endif //STAR_KNIGHT_SHADER_MANAGER_H
