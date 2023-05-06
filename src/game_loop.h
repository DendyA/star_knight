// Created on: 01/05/23.
// Author: DendyA

#ifndef STAR_KNIGHT_GAME_LOOP_H
#define STAR_KNIGHT_GAME_LOOP_H

#include "SDL_events.h"

#include "window_and_user/sk_window.h"
#include "renderer/initializer.h"
#include "renderer/transformation_manager.h"

namespace star_knight
{

    /** GameLoop class\n
     * This class is the runner-type class of the game engine. It is responsible for calling initialization/destruction logic for the
     * SDL and bgfx encapsulating classes, setting up and submitting shaders to the GPU, and running the main game loop.
     */
    class GameLoop final
    {
        public:
            // Typing this as a basic int because this may need to be returned as the return value in main() in the case of an error.
            // And main's signature is obviously an int return type.
            enum SKGameLoopErrCodes: int
            {
                kNoErr = 0,
                kSDLGameObjectsInitErr,
                kbgfxGameObjectsInitErr,
                kShaderManagerProgramGenerateErr
            };

            /** Constructor\n
             * This is the default constructor. Calls initializeSDLGameObjects and initializebgfxGameObjects.
             */
            GameLoop();

            /** Destructor\n
             * This is the default destructor. Will attempt to call m_bgfxInitializer's destroybgfx() if
             * initialization of the m_bgfxInitializer member variable didn't throw specific errors.
             */
            ~GameLoop();

            /** getErrorCode\n
             * Returns the value stored in m_errorCode.
             * The value stored in m_errorCode will always be the resulting status of the most recent failing function call.
             * @return m_errorCode
             */
            SKGameLoopErrCodes getErrorCode();

            /** getErrorMessage\n
             * Returns the value stored in m_errorMessage.
             * The value stored in m_errorCode will always be the resulting message of the most recent failing function call.
             * @return m_errorMessage.
             */
            std::string getErrorMessage();

            /** mainLoop\n
             * This is the main loop which is responsible for everything from initialization, destruction and running of the game engine.
             * @return Ending status of the main game loop. 0 for success, non-zero error code for failure.
             */
            SKGameLoopErrCodes mainLoop();

        private:
            star_knight::GameLoop::SKGameLoopErrCodes m_errorCode;
            std::string m_errorMessage;

            star_knight::SKWindow m_skWindow;
            star_knight::Initializer m_bgfxInitializer;
            star_knight::TransformationManager m_transformManager;

            /** initializeSDLGameObjects\n
             * Initializes all of the SDL game objects required for running the main game loop.
             * @note This function @b MUST be called before initializebgfxGameObjects since that function relies on the results of this one.
             */
            void initializeSDLGameObjects();

            /** initializebgfxGameObjects()\n
             * Initializes all of the bgfx game objects required for running the main game loop.
             * @note This function @b MUST be called after initializeSDLGameObjects since this function relies on the results of that one.
             */
            void initializebgfxGameObjects();

            /** handleKeyDownEvent\n
             * This function handles all supported key down events and calls the relevant functions needed to
             * initiate the action requested by the user pressing the key down.
             * @param keyDownEvent The key down event to be handled.
             */
            void handleKeyDownEvent(SDL_Event keyDownEvent);

            /** saveError\n
             * Saves error status and message.
             * If any of the functions in this class encounter an error, this is called to set the specific message and the errorCode variable.
             * @param prependedToError Error message to save. Expected to be \n and null-terminated.
             * @param errorCode Error code to save. Expected to be one of SKGameLoopErrCodes.
             */
            void saveError(const std::string& prependedToError, SKGameLoopErrCodes errorCode);
    };

} // star_knight

#endif //STAR_KNIGHT_GAME_LOOP_H
