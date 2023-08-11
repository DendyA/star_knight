// Created on: 16/04/23
// Author: DendyA

#include <fstream>
#include <sstream>
#include <iostream>

#include "sk_mesh.h"

#include "shader_manager.h"

star_knight::ShaderManager::ShaderManager() :
    m_currentVertexShader("vs_simple.bin"),
    m_currentFragmentShader("fs_simple.bin"),
    m_currentShadersUpdated(true)
{

}

star_knight::ShaderManager::~ShaderManager() = default;

void
star_knight::ShaderManager::destroyProgramHandle()
{
    // If the program handle is not correctly created, it will be set to kInvalidHandle.
    if(m_errorCode == kNoErr)
    {
        bgfx::destroy(m_program);
    }
}

star_knight::ShaderManager::SKShaderManagerErrCodes
star_knight::ShaderManager::getErrorCode()
{
    return m_errorCode;
}

std::string
star_knight::ShaderManager::getErrorMessage()
{
    return m_errorMessage;
}

std::string
star_knight::ShaderManager::getCurrentVertexShader()
{
    return m_currentVertexShader;
}

void
star_knight::ShaderManager::setCurrentVertexShader(std::string newVertexShader)
{
    m_currentVertexShader = newVertexShader;
    m_currentShadersUpdated = true;
}

std::string
star_knight::ShaderManager::getCurrentFragmentShader()
{
    return m_currentFragmentShader;
}

void
star_knight::ShaderManager::setCurrentFragmentShader(std::string newFragmentShader)
{
    m_currentFragmentShader = newFragmentShader;
    m_currentShadersUpdated = true;
}

void
star_knight::ShaderManager::update(const bgfx::ViewId& viewId, const std::unique_ptr<SKMesh>& mesh, float* transfromMtx, uint64_t renderState)
{
    if(m_currentShadersUpdated)
    {
        bool programGenerationStatus = generateProgram();
        m_currentShadersUpdated = false;

        if(!programGenerationStatus)
        {
            return;
        }
    }

    submitMesh(viewId, mesh, transfromMtx, renderState);
}

bool
star_knight::ShaderManager::generateProgram()
{
    bgfx::ShaderHandle vertexShaderHandle{};
    bgfx::ShaderHandle fragmentShaderHandle{};
    bool success;

    success = loadShader(m_currentVertexShader, kVertexShader, vertexShaderHandle);
    if(!success)
    {
        const std::string errorMsg = "ShaderManager: Error loading vertex shader.";
        saveError(errorMsg, kProgramGenerationErr);

        return success;
    }

    success = loadShader(m_currentFragmentShader, kFragmentShader, fragmentShaderHandle);
    if(!success)
    {
        const std::string errorMsg = "ShaderManager: Error loading fragment shader.";
        saveError(errorMsg, kProgramGenerationErr);

        return success;
    }

    m_program = bgfx::createProgram(vertexShaderHandle, fragmentShaderHandle, true);

    bgfx::destroy(vertexShaderHandle);
    bgfx::destroy(fragmentShaderHandle);

    success = true;

    saveError("", kNoErr);

    return success;
}

// TODO(DendyA): is the bool necessary? I don't capture it in the update() function which is really the only place this would be called?
bool
star_knight::ShaderManager::submitMesh(const bgfx::ViewId& viewId, const std::unique_ptr<SKMesh>& mesh, float* transfromMtx, uint64_t renderState)
{
    bool success = false;

    if(m_errorCode == kProgramGenerationErr)
    {
        const std::string errorMsg = "ShaderManager: Trying to submit mesh with invalid program handle. Not submitting.";
        saveError(errorMsg, ksubmitMeshErr);

        return success;
    }

    if (BGFX_STATE_MASK == renderState)
    {
        renderState = 0 |
                BGFX_STATE_WRITE_RGB |
                BGFX_STATE_WRITE_A |
                BGFX_STATE_WRITE_Z |
                BGFX_STATE_DEPTH_TEST_LESS |
                BGFX_STATE_CULL_CCW |
                BGFX_STATE_MSAA;
    }

    bgfx::setTransform(transfromMtx);
    bgfx::setState(renderState);

    for(const SKMeshInstance& instance : mesh->getInstances())
    {
        bgfx::setVertexBuffer(viewId, instance.m_vBuffHandle);
        bgfx::setIndexBuffer(instance.m_iBuffHandle);

        bgfx::submit(viewId, m_program, 0, BGFX_DISCARD_INDEX_BUFFER | BGFX_DISCARD_VERTEX_STREAMS);
    }

    bgfx::discard();
    success = true;

    return success;
}

bool
star_knight::ShaderManager::loadShader(const std::string& shaderName, ShaderManagerShaderTypes typeIndex, bgfx::ShaderHandle& handle)
{
    bool success = false;
    const std::string fullFilePath = COMPILED_SHADER_PATHS[typeIndex] + shaderName;

    // FIXME(DendyA): Review this and the buffer logic below, technically the shader files are binary.
    std::ifstream shader(fullFilePath); // No need to explicitly call close, ifstream handles this when shader goes out of scope.

    if(!shader.is_open())
    {
        std::cerr << "ShaderManager: File could not be opened: " << fullFilePath << std::endl;
        return success;
    }

    std::stringstream readBuffer;
    readBuffer << shader.rdbuf();

    const std::string shaderData = readBuffer.str();
//  Specifying the size like this should be okay, given that the size of it matches the size of file on disk.
//  Enough memory will be initialized to hold the entirety of the string aka compiled shader file.
    const bgfx::Memory* shaderMem = bgfx::copy((void*)shaderData.c_str(), shaderData.size());

    handle = bgfx::createShader(shaderMem);

//  A third parameter can be used to specify the length of the string but is not necessary here. c_str() returns a const char* to null terminated content
//  meaning that the third parameter can be left as INT32_MAX since doing so, it expects the name parameter to be null terminated.
    bgfx::setName(handle, shaderName.c_str());

    success = true;

    return success;
}

void
star_knight::ShaderManager::saveError(const std::string &errorMessage, SKShaderManagerErrCodes errorCode)
{
    m_errorMessage = errorMessage;
    m_errorCode = errorCode;
}
