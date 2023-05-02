// Created on: 26/03/23
// Author: DendyA

#include "game_loop.h"

int main(int argc, char* args[])
{
    star_knight::GameLoop starKnight = star_knight::GameLoop();
    int exitCondition = starKnight.mainLoop();

    return exitCondition;
}
