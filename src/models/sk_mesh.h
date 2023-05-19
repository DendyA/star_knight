// Created on: 07/05/23.
// Author: DendyA

#ifndef STAR_KNIGHT_SK_MESH_H
#define STAR_KNIGHT_SK_MESH_H

#include <vector>

#include "bx/bounds.h"

#include "bgfx/bgfx.h"

#include "sk_mesh_instance.h"

namespace star_knight
{
    /** SKMesh class\n
     * This class is the main encapsulating mesh object. This class is mainly responsible for "chunk-ing" up the binary data
     * buffer into the specific parameters of the mesh object.
     * @note IMPORTANT: The owning class of this mesh object MUST call destroyHandles before destroying the mesh object
     * to not leak the vertex and index buffer handles.
     */
    class SKMesh
    {
        public:
            /** Constructor\n
             * Default constructor.
             */
            SKMesh();

            /** Destructor\n
             * Default destructor.
             */
            ~SKMesh();

            /** getLayout\n
             * Returns the VertexLayout stored in m_layout.
             * @return The value stored in m_layout.
             */
            const bgfx::VertexLayout& getLayout() const;

            /** getInstances\n
             * Returns the vector of instances stored in m_instances.
             * @return The vector stored in m_instances.
             */
            const std::vector<SKMeshInstance>& getInstances() const;

            /** chunk\n
             * Takes a data buffer of binary mesh data and "chunks" it into multiple SKMeshInstance
             * objects and SKMeshPrimitives objects.
             * @note The passed in data MUST be parsed from a mesh file compiled with bgfx's geometryc tool.
             * @todo Add error checking and pass the status back to the calling function.
             * @todo Add support for the compressed vertex and index chunks.
             * @todo Add error checking to make sure that the data read will NOT go passed the end of the buffer array.
             * @param dataBuff The data to "chunk".
             * @param ramcpy Flag to determine if the vertices and indices should be copied to member variables in an SKMeshInstance object.
             */
            void chunk(const std::vector<unsigned char>& dataBuff, bool ramcpy);

            /** destroyHandles\n
             * Called to destroy the VertexBuffer and IndexBuffer handles stored in the multiple SKMeshInstance objects.
             */
            void destroyHandles();

        private:

            // Object to map VertexLayout attributes to an id which is stored in the mesh file.
            struct MapIDToAttrib
            {
                bgfx::Attrib::Enum attr;
                uint16_t id;
            };

            // List of mapped ids to attributes. The ids used here MUST not be changed. New attributes will have to be
            // added as a new id.
            const std::vector<MapIDToAttrib> idToAttribMap =
            {
                { bgfx::Attrib::Position,  0x0001 },
                { bgfx::Attrib::Normal,    0x0002 },
                { bgfx::Attrib::Tangent,   0x0003 },
                { bgfx::Attrib::Bitangent, 0x0004 },
                { bgfx::Attrib::Color0,    0x0005 },
                { bgfx::Attrib::Color1,    0x0006 },
                { bgfx::Attrib::TexCoord0, 0x0010 },
                { bgfx::Attrib::TexCoord1, 0x0011 },
                { bgfx::Attrib::TexCoord2, 0x0012 },
                { bgfx::Attrib::TexCoord3, 0x0013 },
                { bgfx::Attrib::Indices,   0x000e },
                { bgfx::Attrib::Weight,    0x000f },
                { bgfx::Attrib::TexCoord4, 0x0014 },
                { bgfx::Attrib::TexCoord5, 0x0015 },
                { bgfx::Attrib::TexCoord6, 0x0016 },
                { bgfx::Attrib::TexCoord7, 0x0017 },
                { bgfx::Attrib::Color2,    0x0018 },
                { bgfx::Attrib::Color3,    0x0019 },
            };

            // Object to map VertexLayout attribute types to an id which is stored in the mesh file.
            struct MapIDToAttribType
            {
                bgfx::AttribType::Enum type;
                uint16_t id;
            };

            // List of mapped ids to attribute types. The ids used here MUST not be changed. New attribute types will have to be
            // added as a new id.
            const std::vector<MapIDToAttribType> idToAttribTypeMap =
            {
                { bgfx::AttribType::Uint8,  0x0001 },
                { bgfx::AttribType::Int16,  0x0002 },
                { bgfx::AttribType::Half,   0x0003 },
                { bgfx::AttribType::Float,  0x0004 },
                { bgfx::AttribType::Uint10, 0x0005 },
            };

            bgfx::VertexLayout m_layout;
            std::vector<SKMeshInstance> m_instances;

            /** parseVertexBuffChunk\n
             * Parses the chunk in the binary data related to the vertices. This is the first chunk to be read per SKMeshInstance
             * in the binary mesh data.
             * @todo Add error checking and pass the status back to the calling function.
             * @param instance The current mesh instance to save the vertex buffer information to.
             * @param dataBuff The read in binary data from the mesh file.
             * @param readIndex The index to the next "byte" to read. This IS updated in this function.
             * @param ramcpy Flag to determine if the vertices should be copied to the member variable in the current mesh instance.
             */
            void parseVertexBuffChunk(SKMeshInstance& instance, const std::vector<unsigned char>& dataBuff, uint32_t& readIndex, bool ramcpy);

            /** parseIndexBuffChunk\n
             * Parses the chunk in the binary data related to the indices. This is the second chunk to be read per SKMeshInstance
             * in the binary mesh data.
             * @todo Add error checking and pass the status back to the calling function.
             * @param instance The current mesh instance to save the index buffer information to.
             * @param dataBuff The read in binary data from the mesh file.
             * @param readIndex The index to the next "byte" to read. This IS updated in this function.
             * @param ramcpy Flag to determine if the indices should be copied to the member variable in the current mesh instance.
             */
            void parseIndexBuffChunk(SKMeshInstance& instance, const std::vector<unsigned char>& dataBuff, uint32_t& readIndex, bool ramcpy);

            /** parsePrimitiveChunk\n
             * Parses the chunk(s) in the binary data related to the mesh primitives. This is the third chunk to be read per SKMeshInstance
             * in the binary mesh data. There may be multiple primitive chunks per SKMeshInstance.
             * @todo Add error checking and pass the status back to the calling function.
             * @param instance The current mesh instance to save the read in mesh primitives to. Saved to a member vector in SKMeshInstance.
             * @param dataBuff The read in binary data from the mesh file.
             * @param readIndex The index to the next "byte" to read. This IS updated in this function.
             */
            void parsePrimitiveChunk(SKMeshInstance& instance, const std::vector<unsigned char>& dataBuff, uint32_t& readIndex);

            /** parseVertexLayout\n
             * Parses the VertexLayout in the binary data that exists within the vertex buffer chunk.
             * @todo Add error checking and pass the status back to the calling function.
             * @param layout The current layout object to save the parsed layout into.
             * @param dataBuff The read in binary data from the mesh file.
             * @param readIndex The index to the next "byte" to read. This IS updated in this function.
             */
            void parseVertexLayout(bgfx::VertexLayout &layout, const std::vector<unsigned char> &dataBuff, uint32_t& readIndex);

            /** move\n
             * Does a copy from the dataBuff buffer-ed binary data into the member variable passed in via the templated "store" parameter.
             * Also updates the startingIndex with the number of bytes read.
             * @warning The function makes no attempt to validate that the "store" parameter is large enough to store the number of bytes
             * specified by "numToRead". Nor does it attempt to validate that there is enough data in "dataBuff" (indexing from "startingIndex")
             * to not go off the end of the vector.
             * @tparam Ty Used as a generic for any variable type passed-in by reference as the "store" parameter.
             * @param dataBuff The read in binary data from the mesh file.
             * @param store The store object to copy the memory into. Of templated type Ty which is a generic type.
             * @param startingIndex The index to the start reading from. This IS updated in this function.
             * @param numToRead Number of "bytes" to read.
             */
            template<typename Ty>
            void move(const std::vector<unsigned char> &dataBuff, Ty& store, uint32_t& startingIndex, const uint32_t& numToRead);

            /** fill\n
             * Does a copy from the dataBuff buffer-ed binary data into the pointer passed in via the void* "store" parameter.
             * Also updates the startingIndex with the number of bytes read. The only difference between this and move(), is that
             * "store" is a void* parameter rather than a templated generic type.
             * @warning The function makes no attempt to validate that the "store" parameter is large enough to store the number of bytes
             * specified by "numToRead". Nor does it attempt to validate that there is enough data in "dataBuff" (indexing from "startingIndex")
             * to not go off the end of the vector.
             * @param dataBuff The read in binary data from the mesh file.
             * @param store The pointer to copy the memory into. Of type void* since the type is not relevant when doing a memcopy.
             * @param startingIndex The index to the start reading from. This IS updated in this function.
             * @param numToRead Number of "bytes" to read.
             */
            void fill(const std::vector<unsigned char> &dataBuff, void* store, uint32_t& startingIndex, const uint32_t& numToRead);

            /** convertIDToAttrib\n
             * Parses through const array idToAttribMap to determine which attribute the id corresponds to.
             * @param id The id to map to an attribute.
             * @return The attribute the given id corresponds to. Otherwise returns bgfx::Attrib::Count if the id did not map to an attribute.
             */
            bgfx::Attrib::Enum convertIDToAttrib(uint16_t id);

            /** convertIDToAttribType\n
             * Parses through const array idToAttribTypeMap to determine which attribute type the id corresponds to.
             * @param id The id to map to an attribute type.
             * @return The attribute type the given id corresponds to. Otherwise returns bgfx::AttribType::Count if the id did not map to an attribute type.
             */
            bgfx::AttribType::Enum convertIDToAttribType(uint16_t id);
    };

} // star_knight

#endif //STAR_KNIGHT_SK_MESH_H
