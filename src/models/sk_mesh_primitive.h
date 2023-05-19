// Created on: 07/05/23.
// Author: DendyA

#ifndef STAR_KNIGHT_SK_MESH_PRIMITIVE_H
#define STAR_KNIGHT_SK_MESH_PRIMITIVE_H

#include <cstdint>

#include "bx/bounds.h"

namespace star_knight
{

    /** SKMeshPrimitive class\n
     * This class is used by the SKMeshInstance class to hold the various parameters of each primitive in the mesh.
     * @note IMPORTANT: The parameters MUST be of the types shown here. Otherwise, the parsing of the mesh file will break;
     */
    class SKMeshPrimitive
    {
        public:
            uint32_t m_startIndex;
            uint32_t m_numIndices;
            uint32_t m_startVertex;
            uint32_t m_numVertices;

            bx::Sphere m_sphere;
            bx::Aabb m_aabb;
            bx::Obb m_obb;

            /** Constructor\n
             * Calls resetToDefault() to set all parameters to their defaults.
             */
            SKMeshPrimitive();

            /** Destructor\n
             * Default destructor.
             */
            ~SKMeshPrimitive();

            /** resetToDefault\n
             * A helper function used to set all parameters to their default state.
             */
            void resetToDefault();
    };

} // star_knight

#endif //STAR_KNIGHT_SK_MESH_PRIMITIVE_H
