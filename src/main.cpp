// Created on: 26/03/23
// Author: DendyA

#include <iostream>

#include "game_loop.h"

int main(int argc, char* args[])
{
    star_knight::GameLoop starKnight = star_knight::GameLoop();

    if(starKnight.getErrorCode() != star_knight::GameLoop::kNoErr)
    {
        std::cerr << starKnight.getErrorMessage() << std::endl;
        return starKnight.getErrorCode();
    }

    return starKnight.mainLoop();
}
