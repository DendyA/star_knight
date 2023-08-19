// Created on: 16/04/23
// Author: DendyA

#ifndef STAR_KNIGHT_SHADER_MANAGER_H
#define STAR_KNIGHT_SHADER_MANAGER_H

#include <vector>

#include "bgfx/bgfx.h"

#include "sk_mesh.h"

namespace star_knight
{
    /** ShaderManager class\n
     * The ShaderManager class is responsible for loading and initializing the vertex and fragment shaders,
     * creating the vertex and index buffers, and generating the shader programs from the vertex and fragment shaders.
     * All functions are static given that they don't rely on any member variables.
     * @todo I believe that the shader manager class will change drastically when the entity system is implemented. I think EACH entity (or mesh)
     * will need a shader manager that controls its own shaders and meshes etc. Will probably need the update function to be called from the entity's update
     * function and the main loop function will simply iterate over each mesh and call update.
     */
    class ShaderManager final
    {
        public:
            // Typing this as a basic int because this may need to be returned as the return value in main() in the case of an error.
            // And main's signature is obviously an int return type.
            enum SKShaderManagerErrCodes: int
            {
                kNoErr = 0,
                kProgramGenerationErr,
                ksubmitMeshErr
            };

            /** Constructor\n
            * The default constructor of the ShaderManager class.
            */
            ShaderManager();

            /** Destructor\n
             * The destructor of the ShaderManager class.
             */
            ~ShaderManager();

            /** getErrorCode\n
            * Returns the value stored in m_errorCode.
            * The value stored in m_errorCode will always be the resulting status of the most recent failing function call.
            * @return m_errorCode
            */
            star_knight::ShaderManager::SKShaderManagerErrCodes getErrorCode();

            /** getErrorMessage\n
             * Returns the value stored in m_errorMessage.
             * The value stored in m_errorCode will always be the resulting message of the most recent failing function call.
             * @return m_errorMessage.
             */
            std::string getErrorMessage();

            /** getCurrentVertexShader\n
             * Returns the value stored in m_currentVertexShader.
             * @return m_currentVertexShader
             */
            std::string getCurrentVertexShader();

            /** setCurrentVertexShader
             * Sets a new vertex shader filename and stores it in m_currentVertexShader.
             * @param newVertexShader the filename for the new vertex shader
             */
            void setCurrentVertexShader(std::string newVertexShader);

            /** getCurrentFragmentShader\n
             * Returns the value stored in m_currentFragmentShader.
             * @return m_currentFragmentShader
             */
            std::string getCurrentFragmentShader();

            /** setCurrentFragmentShader\n
             * Sets a new fragment shader filename and stores it in m_currentFragmentShader.
             * @param newFragmentShader the filename for the new fragment shader
             */
            void setCurrentFragmentShader(std::string newFragmentShader);

            /** destoryProgramHandle\n
             * Destroys the program handle saved as a member variable in m_program. This
             * needs to be called rather than relying on the destructor to be called due to
             * a destruction of the bgfx subsystem timing issue.
             */
            void destroyProgramHandle();

            /** update\n
             * This function is called in GameLoop's main loop function to update/regenerate program handles based
             * on updates to the saved shaders, and also takes the necessary parameters to call submitMesh().
             * @todo The call to submitMesh every tick slows down the ability to navigate around the scene. Need to update it to ONLY call when something changes.
             * This should probably be handled when the above note about shader manager instances being tied to each entity is resolved.
             * @param viewId The id of the view to render the mesh to
             * @param mesh The mesh to render
             * @param transfromMtx Transformation matrix to apply to the mesh
             * @param renderState The render state.
             */
            void update(const bgfx::ViewId& viewId, const std::unique_ptr<SKMesh>& mesh, float* transfromMtx, uint64_t renderState);

            /** generateProgram\n
             * This program loads the shader files stored in member variables m_currentVertexShader and m_currentFragmentShader,
             * creates the shader program from these and stores it in the member variable m_program.
             * It internally calls the loadShader() function which is responsible for reading the shader from disk.
             * @warning @b MUST be called before submitMesh.
             * @note It destroys the two loaded shader handles once they have been loaded into the shader program.
             * @return The result of running this function. True for success, false otherwise.
             */
            bool generateProgram();

            /** submitMesh\n
             * This function takes a mesh and program handle and submits it for rendering. The transformation matrix to apply to the
             * model is also supplied and set in this function. Uses m_program as the program to set with the mesh.
             * @param viewId The id of the view to render the mesh to.
             * @param mesh The mesh to render.
             * @param transfromMtx Transformation matrix to apply to the mesh
             * @param renderState The render state.
             */
            bool submitMesh(const bgfx::ViewId& viewId, const std::unique_ptr<SKMesh>& mesh, float* transfromMtx, uint64_t renderState);

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

            bgfx::ProgramHandle m_program{};

            // Filename paths to the currently used shaders.
            std::string m_currentVertexShader;
            std::string m_currentFragmentShader;

            // Flag to know if the shaders were updated.
            bool m_currentShadersUpdated;

            star_knight::ShaderManager::SKShaderManagerErrCodes m_errorCode;
            std::string m_errorMessage;

            /** loadShader\n
             * Reads-in a shader from disk and loads it into a ShaderHandle object.
             * @param shaderName The path to the shader file on disk.
             * @param typeIndex One of ShaderManagerShaderTypes for the type of shader pointed to by parameter shaderName.
             * @param handle The ShaderHandle to save the loaded shader to.
             * @return The result of running this function. True for success, false otherwise.
             */
            static bool loadShader(const std::string& shaderName, ShaderManagerShaderTypes typeIndex, bgfx::ShaderHandle& handle);

            /** saveError\n
            * Saves error status and message.
            * If any of the functions in this class encounter an error, this is called to set the specific message and the errorCode variable.
            * @param prependedToError Error message to save. Expected to be \n and null-terminated.
            * @param errorCode Error code to save. Expected to be one of SKShaderManagerErrCodes.
            */
            void saveError(const std::string& prependedToError, SKShaderManagerErrCodes errorCode);
    };
} // star_knight

#endif //STAR_KNIGHT_SHADER_MANAGER_H
