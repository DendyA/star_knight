// Created on: 16/04/23
// Author: DendyA

#include <fstream>
#include <sstream>
#include <iostream>

#include "sk_mesh.h"

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

bool
star_knight::ShaderManager::loadShader(const std::string& shaderName, ShaderManagerShaderTypes typeIndex, bgfx::ShaderHandle& handle)
{
    bool success = false;
    const std::string fullFilePath = COMPILED_SHADER_PATHS[typeIndex] + shaderName;

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

bool
star_knight::ShaderManager::generateProgram(const std::string& vertexShaderName, const std::string& fragmentShaderName, bgfx::ProgramHandle& program)
{
    bgfx::ShaderHandle vertexShaderHandle{};
    bgfx::ShaderHandle fragmentShaderHandle{};
    bool success;

    success = loadShader(vertexShaderName, kVertexShader, vertexShaderHandle);
    if(!success)
    {
        std::cerr << "ShaderManager: Error loading shader." << std::endl;
        return success;
    }

    success = loadShader(fragmentShaderName, kFragmentShader, fragmentShaderHandle);
    if(!success)
    {
        std::cerr << "ShaderManager: Error loading shader." << std::endl;
        return success;
    }

    program = bgfx::createProgram(vertexShaderHandle, fragmentShaderHandle, true);

    bgfx::destroy(vertexShaderHandle);
    bgfx::destroy(fragmentShaderHandle);

    success = true;

    return success;
}

void
star_knight::ShaderManager::submitMesh(const bgfx::ViewId& viewId, const std::unique_ptr<SKMesh>& mesh, const bgfx::ProgramHandle& program, float* transfromMtx, uint64_t renderState)
{
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

        bgfx::submit(viewId, program, 0, BGFX_DISCARD_INDEX_BUFFER | BGFX_DISCARD_VERTEX_STREAMS);
    }

    bgfx::discard();
}
