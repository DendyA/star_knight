// Created on: 07/05/23.
// Author: DendyA

#include "sk_mesh_instance.h"

star_knight::SKMeshInstance::SKMeshInstance()
{
    resetToDefault();
}

star_knight::SKMeshInstance::~SKMeshInstance() = default;

void
star_knight::SKMeshInstance::resetToDefault()
{
    m_vBuffHandle.idx = bgfx::kInvalidHandle;
    m_iBuffHandle.idx = bgfx::kInvalidHandle;

    m_numOfVertices = 0;
    m_vertices = nullptr;
    m_numOfIndices = 0;
    m_indices = nullptr;

    m_primitives.clear();
}

void
star_knight::SKMeshInstance::destroyHandles()
{
    bgfx::destroy(m_vBuffHandle);
    bgfx::destroy(m_iBuffHandle);
}
