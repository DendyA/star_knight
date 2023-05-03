// Created on: 01/05/23.
// Author: DendyA

#ifndef STAR_KNIGHT_SK_GLOBAL_DEFINES_H
#define STAR_KNIGHT_SK_GLOBAL_DEFINES_H

#include <cstdint>

namespace star_knight
{
    static const uint32_t STARTING_SCREEN_WIDTH = 1280u;
    static const uint32_t STARTING_SCREEN_HEIGHT = 1024u;

    static constexpr float STARTING_FOV = 60.0f;
    static constexpr float STARTING_ASPECT_RATIO = float(STARTING_SCREEN_WIDTH) / float(STARTING_SCREEN_WIDTH);
    static constexpr float STARTING_NEAR_PLANE = 0.01f;
    static constexpr float STARTING_FAR_PLANE = 100.0f;
}

#endif //STAR_KNIGHT_SK_GLOBAL_DEFINES_H
