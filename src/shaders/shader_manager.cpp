// Created on: 16/04/23
// Author: DendyA

#include <fstream>
#include <sstream>

#include "bgfx.h"

#include "shader_manager.h"

// TODO(DendyA): This should be put elsewhere and included in this class.
struct PosColorVertex
{
//  Position data
    float m_x;
    float m_y;
    float m_z;

    uint32_t m_abgr; // Colour value

    static void init()
    {
        ms_decl
                .begin()
                .add(bgfx::Attrib::Position, 3, bgfx::AttribType::Float)
                .add(bgfx::Attrib::Color0, 4, bgfx::AttribType::Uint8, true)
                .end();
    };

    static bgfx::VertexLayout ms_decl;
};

bgfx::VertexLayout PosColorVertex::ms_decl;

// TODO(DendyA): The current value of shaderName is the full path to the file on disk.
//      In future revisions, this should just be the name of the file not the full path.
//      The full path should probably be defined in this class and then pre-pended to the filename.
bgfx::ShaderHandle
ShaderManager::loadShader(const std::string& shaderName)
{
//    TODO(DendyA): Need to add error checking here.
    std::ifstream shader(shaderName);
    std::stringstream readBuffer;
    readBuffer << shader.rdbuf();

//    TODO(DendyA): This needs to be tested. Especially the pointer to the shaderData passed into copy().
    const std::string shaderData = readBuffer.str();
    const bgfx::Memory* shaderMem = bgfx::copy(shaderData.c_str(), shaderData.size());

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

    bgfx::destroy(vertexShaderHandle);
    bgfx::destroy(fragmentShaderHandle);

    return program;
}

bgfx::VertexBufferHandle
ShaderManager::initVertexBuffer()
{
//    TODO(DendyA): This needs to be removed in future revisions of the code. Should be read-in from a file and then passed-in here.
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
//    TODO(DendyA): This needs to be removed in future revisions of the code. Should be read-in from a file and then passed-in here.
    static const uint16_t s_cubeTriList[] =
    {
            0,1,3,
            1,2,3
    };

    bgfx::IndexBufferHandle indexBufferHandle = bgfx::createIndexBuffer(bgfx::makeRef(s_cubeTriList, sizeof(s_cubeTriList)));

    return indexBufferHandle;
}