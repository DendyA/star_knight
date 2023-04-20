// Created on: 16/04/23
// Author: DendyA

#ifndef STAR_KNIGHT_SHADER_MANAGER_H
#define STAR_KNIGHT_SHADER_MANAGER_H

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

class ShaderManager
{
    public:
        static bgfx::ProgramHandle generateProgram(const std::string& vertexShaderName, const std::string& fragmentShaderName);
        static bgfx::VertexBufferHandle initVertexBuffer();
        static bgfx::IndexBufferHandle initIndexBuffer();

    private:
        static bgfx::ShaderHandle loadShader(const std::string& shaderName);
};


#endif //STAR_KNIGHT_SHADER_MANAGER_H
