// Created on: 07/05/23.
// Author: DendyA

#include <fstream>
#include <vector>
#include <iostream>

#include "sk_parser.h"

bool
star_knight::SKParser::openMeshFile(const std::string& filePath, std::unique_ptr<SKMesh>& mesh)
{
    bool success = false;
    std::ifstream meshFile(filePath, std::ios::binary);

    if(!meshFile.is_open())
    {
        std::cerr << "SKParser: File could not be opened: " << filePath << std::endl;
        return success;
    }

    // Buffer of unsigned chars to hold the contents of the binary mesh file.
    std::vector<unsigned char> buffer(std::istreambuf_iterator<char>(meshFile), {});

    // Creates a new dynamically allocated object on the heap.
    mesh = std::make_unique<SKMesh>();

    // If the unique_ptr couldn't be created, leave with error.
    if(mesh == nullptr)
    {
        return success;
    }

    // Read all chunks of the binary mesh file.
    mesh->chunk(buffer, true);

    success = true;
    return success;
}
