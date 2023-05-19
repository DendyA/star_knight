// Created on: 07/05/23.
// Author: DendyA

#include <cstring>
#include <string>

#include "sk_mesh.h"

star_knight::SKMesh::SKMesh() = default;

star_knight::SKMesh::~SKMesh() = default;

const bgfx::VertexLayout&
star_knight::SKMesh::getLayout() const
{
    return m_layout;
}

const std::vector<star_knight::SKMeshInstance>&
star_knight::SKMesh::getInstances() const
{
    return m_instances;
}

void
star_knight::SKMesh::destroyHandles()
{
    for(SKMeshInstance& instance : m_instances)
    {
        instance.destroyHandles();
    }
}


void
star_knight::SKMesh::chunk(const std::vector<unsigned char>& dataBuff, bool ramcpy)
{
    // Each section in the binary mesh file starts with these special numbers.
    constexpr uint32_t vertexBuffChunkMagicNum = BX_MAKEFOURCC('V', 'B', ' ', 0x1);
    constexpr uint32_t indexBuffChunkMagicNum  = BX_MAKEFOURCC('I', 'B', ' ', 0x0);
    constexpr uint32_t primitiveChunkMagicNum  = BX_MAKEFOURCC('P', 'R', 'I', 0x0);

    SKMeshInstance instance;

    uint32_t readIndex = 0u;
    uint32_t chunk = 0u;

    while(readIndex < dataBuff.size())
    {
        // Important: This move, for reading the chunk, also progresses the read index!
        // Otherwise, the parsing of each section in the file would start at the wrong index.
        move(dataBuff, chunk, readIndex, sizeof(chunk));

        switch (chunk)
        {
            case vertexBuffChunkMagicNum:
                parseVertexBuffChunk(instance, dataBuff, readIndex, ramcpy);
                break;
            case indexBuffChunkMagicNum:
                parseIndexBuffChunk(instance, dataBuff, readIndex, ramcpy);
                break;
            case primitiveChunkMagicNum:
                parsePrimitiveChunk(instance, dataBuff, readIndex);
                m_instances.push_back(instance);
                instance.resetToDefault();
                break;
            default:
                // TODO(DendyA): What to do on a default case?
                break;
        }
    }
}

void
star_knight::SKMesh::parseVertexBuffChunk(SKMeshInstance& instance, const std::vector<unsigned char>& dataBuff, uint32_t& readIndex, bool ramcpy)
{
    // The order of parsing the buffer is critical!
    // It must be done in this order since the readIndex is used to read sequentially through the buffer.
    move(dataBuff, instance.m_sphere, readIndex, sizeof(instance.m_sphere));
    move(dataBuff, instance.m_aabb, readIndex, sizeof(instance.m_aabb));
    move(dataBuff, instance.m_obb, readIndex, sizeof(instance.m_obb));

    parseVertexLayout(m_layout, dataBuff, readIndex);

    uint16_t stride = m_layout.getStride();

    move(dataBuff, instance.m_numOfVertices, readIndex, sizeof(instance.m_numOfVertices));

    // The reason for the sizeof(uint8_t) is to include the size of each vertex.
    const uint32_t sizeOfVertexMemory = instance.m_numOfVertices * stride * sizeof(uint8_t);

    // Copy the vertices into bgfx::Memory to make a vertex buffer out of them.
    const bgfx::Memory* mem = bgfx::copy((dataBuff.data() + readIndex), sizeOfVertexMemory);

    readIndex += sizeOfVertexMemory;

    if(ramcpy)
    {
        // TODO(DendyA): Add allocation of memory to hold the vertices currently in bgfx::Memory.
    }

    instance.m_vBuffHandle = bgfx::createVertexBuffer(mem, m_layout);
}

void
star_knight::SKMesh::parseIndexBuffChunk(SKMeshInstance& instance, const std::vector<unsigned char>& dataBuff, uint32_t& readIndex, bool ramcpy)
{
    // The order of parsing the buffer is critical!
    // It must be done in this order since the readIndex is used to read sequentially through the buffer.
    move(dataBuff, instance.m_numOfIndices, readIndex, sizeof(instance.m_numOfIndices));

    // The reason for the sizeof(uint16_t) is to include the size of each index.
    const uint32_t sizeOfIndexMemory = instance.m_numOfIndices * sizeof(uint16_t);

    // Copy the indices into bgfx::Memory to make an index buffer out of them.
    const bgfx::Memory* mem = bgfx::copy((dataBuff.data() + readIndex), sizeOfIndexMemory);

    readIndex += sizeOfIndexMemory;

    if(ramcpy)
    {
        // TODO(DendyA): Add allocation of memory to hold the indices currently in bgfx::Memory.
    }

    instance.m_iBuffHandle = bgfx::createIndexBuffer(mem);
}

void
star_knight::SKMesh::parsePrimitiveChunk(SKMeshInstance& instance, const std::vector<unsigned char>& dataBuff, uint32_t& readIndex)
{
    // The order of parsing the buffer is critical!
    // It must be done in this order since the readIndex is used to read sequentially through the buffer.
    uint16_t length;
    move(dataBuff, length, readIndex, sizeof(length));

    std::string meshMaterial(length, ' ');
    fill(dataBuff, meshMaterial.data(), readIndex, length);

    uint16_t numberOfPrims;
    move(dataBuff, numberOfPrims, readIndex, sizeof(numberOfPrims));

    for(uint16_t i = 0; i < numberOfPrims; ++i)
    {
        move(dataBuff, length, readIndex, sizeof(length));

        std::string primitiveName(length, ' ');
        fill(dataBuff, primitiveName.data(), readIndex, length);

        SKMeshPrimitive primitive;

        move(dataBuff, primitive.m_startIndex, readIndex, sizeof(primitive.m_startIndex));
        move(dataBuff, primitive.m_numIndices, readIndex, sizeof(primitive.m_numIndices));
        move(dataBuff, primitive.m_startVertex, readIndex, sizeof(primitive.m_startVertex));
        move(dataBuff, primitive.m_numVertices, readIndex, sizeof(primitive.m_numVertices));
        move(dataBuff, primitive.m_sphere, readIndex, sizeof(primitive.m_sphere));
        move(dataBuff, primitive.m_aabb, readIndex, sizeof(primitive.m_aabb));
        move(dataBuff, primitive.m_obb, readIndex, sizeof(primitive.m_obb));

        instance.m_primitives.push_back(primitive);
    }
}

void
star_knight::SKMesh::parseVertexLayout(bgfx::VertexLayout &layout, const std::vector<unsigned char> &dataBuff, uint32_t& readIndex)
{
    // The order of parsing the buffer is critical!
    // It must be done in this order since the readIndex is used to read sequentially through the buffer.
    uint8_t numAttrs;
    move(dataBuff, numAttrs, readIndex, sizeof(numAttrs));

    uint16_t stride;
    move(dataBuff, stride, readIndex, sizeof(stride));

    layout.begin();

    for(uint8_t i = 0; i < numAttrs; ++i)
    {
        uint16_t offset;
        move(dataBuff, offset, readIndex, sizeof(offset));

        uint16_t attribId;
        move(dataBuff, attribId, readIndex, sizeof(attribId));

        uint8_t numOfElements;
        move(dataBuff, numOfElements, readIndex, sizeof(numOfElements));

        uint16_t attribTypeId;
        move(dataBuff, attribTypeId, readIndex, sizeof(attribTypeId));

        bool normalized;
        move(dataBuff, normalized, readIndex, sizeof(normalized));

        bool asInt;
        move(dataBuff, asInt, readIndex, sizeof(asInt));

        bgfx::Attrib::Enum attr = convertIDToAttrib(attribId);
        bgfx::AttribType::Enum type = convertIDToAttribType(attribTypeId);

        if((attr != bgfx::Attrib::Count) && (type != bgfx::AttribType::Count))
        {
            layout.add(attr, numOfElements, type, normalized, asInt);
            layout.m_offset[attr] = offset;
        }
    }

    layout.end();
    layout.m_stride = stride;
}

template<typename Ty>
void
star_knight::SKMesh::move(const std::vector<unsigned char>& dataBuff, Ty& store, uint32_t& startingIndex, const uint32_t& numToRead)
{
    std::memcpy(&store, (dataBuff.data() + startingIndex), numToRead);
    startingIndex += numToRead;
}

void
star_knight::SKMesh::fill(const std::vector<unsigned char>& dataBuff, void* store, uint32_t& startingIndex, const uint32_t& numToRead)
{
    std::memcpy(&store, (dataBuff.data() + startingIndex), numToRead);
    startingIndex += numToRead;
}

bgfx::Attrib::Enum
star_knight::SKMesh::convertIDToAttrib(uint16_t id)
{
    for(MapIDToAttrib pair : idToAttribMap)
    {
        if(pair.id == id)
        {
            return pair.attr;
        }
    }

    return bgfx::Attrib::Count;
}

bgfx::AttribType::Enum
star_knight::SKMesh::convertIDToAttribType(uint16_t id)
{
    for(MapIDToAttribType pair : idToAttribTypeMap)
    {
        if(pair.id == id)
        {
            return pair.type;
        }
    }

    return bgfx::AttribType::Count;
}
