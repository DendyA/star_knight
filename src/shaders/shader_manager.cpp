// Created on: 16/04/23
// Author: DendyA

#include <fstream>
#include <sstream>

#include "bgfx.h"

#include "shader_manager.h"

bgfx::ShaderHandle
ShaderManager::loadShader(const std::string& shaderName)
{
//    TODO(DendyA): Need to add error checking here.
    std::ifstream shader(shaderName);
    std::stringstream readBuffer;
    readBuffer << shader.rdbuf();

//    TODO(DendyA): This needs to be tested. Especially the pointer to the shaderData passed into copy().
    const std::string shaderData = readBuffer.str();
    const bgfx::Memory* shaderMem = bgfx::copy(&shaderData, shaderData.size());

    bgfx::ShaderHandle handle = bgfx::createShader(shaderMem);
    // TODO(DendyA): According to the documentation, the third parameter being INT32_MAX (default) means it expects the string to be null terminated.
    //      While this is true, I want to refactor later and see if the length should be defined.
    bgfx::setName(handle, shaderName.c_str());

    return handle;
}

bgfx::ProgramHandle
ShaderManager::generateProgram(const std::string& vertexShaderName, const std::string& fragmentShaderName)
{
    bgfx::ShaderHandle vertexShaderHandle = loadShader(vertexShaderName);
    bgfx::ShaderHandle fragmentShaderHandle = loadShader(fragmentShaderName);

    bgfx::ProgramHandle program = bgfx::createProgram(vertexShaderHandle, fragmentShaderHandle, true);

    return program;
}

bgfx::VertexBufferHandle
ShaderManager::initVertexBuffer()
{
    static PosColorVertex s_cubeVertices[] =
    {
    {  0.5f,  0.5f, 0.0f, 0xff0000ff },
    {  0.5f, -0.5f, 0.0f, 0xff0000ff },
    { -0.5f, -0.5f, 0.0f, 0xff00ff00 },
    { -0.5f,  0.5f, 0.0f, 0xff00ff00 }
    };

    PosColorVertex::init();

    bgfx::VertexBufferHandle vertexBufferHandle =
            bgfx::createVertexBuffer(bgfx::makeRef(s_cubeVertices, sizeof(s_cubeVertices)),PosColorVertex::ms_decl);

    return vertexBufferHandle;
}

bgfx::IndexBufferHandle
ShaderManager::initIndexBuffer()
{
    static const uint16_t s_cubeTriList[] =
    {
            0,1,3,
            1,2,3
    };

    bgfx::IndexBufferHandle indexBufferHandle = bgfx::createIndexBuffer(bgfx::makeRef(s_cubeTriList, sizeof(s_cubeTriList)));

    return indexBufferHandle;
}