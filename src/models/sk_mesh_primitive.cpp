// Created on: 07/05/23.
// Author: DendyA

#include "sk_mesh_primitive.h"

star_knight::SKMeshPrimitive::SKMeshPrimitive()
{
    resetToDefault();
}

star_knight::SKMeshPrimitive::~SKMeshPrimitive() = default;

void
star_knight::SKMeshPrimitive::resetToDefault()
{
    m_startIndex = 0u;
    m_numIndices = 0u;
    m_startVertex = 0u;
    m_numVertices = 0u;

    // m_sphere
    m_sphere.center = bx::init::None;
    m_sphere.radius = 0.0f;

    // m_aabb
    m_aabb.max = bx::init::None;
    m_aabb.min = bx::init::None;

    // m_obb
    for(float& i : m_obb.mtx)
    {
        i = 0;
    }
}
