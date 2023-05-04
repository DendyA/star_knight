// Created on: 01/05/23.
// Author: DendyA

#ifndef STAR_KNIGHT_GAME_LOOP_H
#define STAR_KNIGHT_GAME_LOOP_H

namespace star_knight
{

    /** GameLoop class\n
     * This class is the runner-type class of the game engine. It is responsible for calling initialization/destruction logic for the
     * SDL and bgfx encapsulating classes, setting up and submitting shaders to the GPU, and running the main game loop.
     * @todo Perhaps move initialization of SDL and bgfx class objects into the constructor and follow a similar pattern to the sk_window class?
     */
    class GameLoop
    {
        public:
            /** Constructor\n
             * This is the default constructor.
             */
            GameLoop();

            /** Destructor\n
             * This is the default destructor.
             */
            ~GameLoop();

            /** mainLoop\n
             * This is the main loop which is responsible for everything from initialization, destruction and running of the game engine.
             * @return Ending status of the main game loop. 0 for success, non-zero error code for failure.
             */
            int mainLoop();
    };

} // star_knight

#endif //STAR_KNIGHT_GAME_LOOP_H
