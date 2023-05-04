// Created on: 16/04/23
// Author: DendyA

#ifndef STAR_KNIGHT_SHADER_MANAGER_H
#define STAR_KNIGHT_SHADER_MANAGER_H

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
             * This program loads the two passed in shader files and creates the shader program from these and returns it to the caller.
             * It internally calls the loadShader() function which is responsible for reading the shader from disk.
             * @note It destroys the two loaded shader handles once it has been loaded into the shader program.
             * @param vertexShaderName The path to the vertex shader file on disk.
             * @param fragmentShaderName The path to the vertex shader file on disk.
             * @return The program handle for the created shader program.
             */
            static bgfx::ProgramHandle generateProgram(const std::string& vertexShaderName, const std::string& fragmentShaderName);

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
            /** loadShader\n
             * Reads-in a shader from disk and loads it into a ShaderHandle object.
             * @param shaderName The path to the shader file on disk.
             * @return The shader handle to the loaded and initialized shader.
             */
            static bgfx::ShaderHandle loadShader(const std::string& shaderName);
    };
} // star_knight

#endif //STAR_KNIGHT_SHADER_MANAGER_H
