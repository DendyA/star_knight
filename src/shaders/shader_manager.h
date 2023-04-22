// Created on: 16/04/23
// Author: DendyA

#ifndef STAR_KNIGHT_SHADER_MANAGER_H
#define STAR_KNIGHT_SHADER_MANAGER_H

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
