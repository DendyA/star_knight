// Created on: 07/05/23.
// Author: DendyA

#ifndef STAR_KNIGHT_SK_MESH_INSTANCE_H
#define STAR_KNIGHT_SK_MESH_INSTANCE_H

#include <vector>
#include <memory>

#include "bgfx/bgfx.h"

#include "bx/bounds.h"

#include "sk_mesh_primitive.h"

namespace star_knight
{
    /** SKMeshInstance class\n
     * This class is used to hold each instance of the groupings of indices and vertices that exist in the mesh. There may be one or more instances
     * per mesh. And each instance can have one or more primitives. Since each instance has a set of vertices and indices, a relevant VertexBuffer
     * and IndexBuffer are created.
     * @note IMPORTANT: The parameters MUST be of the types shown here. Otherwise, the parsing of the mesh file will break.
     */
    class SKMeshInstance
    {
        public:
            bgfx::VertexBufferHandle m_vBuffHandle;
            bgfx::IndexBufferHandle m_iBuffHandle;

            uint16_t m_numOfVertices;
            // If this is a std::unique_ptr, the push_back of this object to the SKMeshInstance vector in SKMesh
            // will throw an error since std::unique_ptr has a deleted copy constructor. Same for m_indices.
            // TODO: A copy constructor for this class should be created and then the std::unique_ptr can be released to the new object.
            uint8_t* m_vertices;
            uint32_t m_numOfIndices;
            uint16_t* m_indices;

            bx::Sphere m_sphere;
            bx::Aabb m_aabb; // Axis-aligned bounding box.
            bx::Obb m_obb;
            std::vector<SKMeshPrimitive> m_primitives;

            /** Constructor\n
             * Calls resetToDefault() to set all parameters to their defaults.
             */
            SKMeshInstance();

            /** Destructor\n
             * Default destructor.
             */
            ~SKMeshInstance();

            /** resetToDefault\n
             * A helper function used to set all parameters to their default state.
             */
            void resetToDefault();

            /** destroyHandles\n
             * A helper function used to destroy buffer vertex and index buffer handles.
             */
            void destroyHandles();
    };
} // star_knight

#endif //STAR_KNIGHT_SK_MESH_INSTANCE_H
