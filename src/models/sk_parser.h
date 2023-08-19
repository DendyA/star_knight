// Created on: 07/05/23.
// Author: DendyA

#ifndef STAR_KNIGHT_SK_PARSER_H
#define STAR_KNIGHT_SK_PARSER_H

#include <memory>

#include "sk_mesh.h"

namespace star_knight
{
    /** SKParser class\n
     * This class is responsible for reading in a mesh file provided at a given filePath. Its only member function
     * is defined as static so any class that needs to can import a mesh file without needing to instantiate the parser.
     */
    class SKParser final
    {
        public:
            /** openMeshFile\n
             * The main function used to open and import the binary mesh file. Defined static such that any class can
             * read in mesh files.
             * @note The mesh file to be imported MUST be compiled with bgfx's geometryc tool.
             * @param filePath File path to the binary mesh file compiled with bgfx's geometryc tool.
             * @param mesh The mesh object that will hold the read in mesh.
             * @return The status of opening and parsing the file. True for success, false otherwise.
             */
            static bool openMeshFile(const std::string& filePath, std::unique_ptr<SKMesh>& mesh);
    };
} // star_knight

#endif //STAR_KNIGHT_SK_PARSER_H
