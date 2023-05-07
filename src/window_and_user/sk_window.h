// Created on: 24/04/23.
// Author: DendyA

#ifndef STAR_KNIGHT_WINDOW_AND_INPUT_H
#define STAR_KNIGHT_WINDOW_AND_INPUT_H

#include <string>

#include "SDL.h"

namespace star_knight
{
    /** SKWindow class\n
     * The SKWindow class is responsible for the initialization / destruction of SDL, an SDL window, and any other SDL sub-components.
     */
    class SKWindow final
    {
        public:
            // Typing this as a basic int because this may need to be returned as the return value in main() in the case of an error.
            // And main's signature is obviously an int return type.
            enum SKWindowErrCodes: int
            {
                kNoErr = 0,
                kSDLInitErr,
                kSDLWinCreateErr
            };

            /** Constructor\n
             * The main constructor of the SKWindow class. Calls initSDL.
             */
            SKWindow();

            /** Destructor\n
             * The main destructor of the SKWindow class. Calls destroySDL.
             */
            ~SKWindow();

            /** getpwindow\n
             * Returns the instance of SDL_Window saved as a member variable in this class.
             * m_pwindow will be a nullptr if this class did not initialize properly.
             * @return m_pwindow
             */
            SDL_Window* getpwindow();

            /** getErrorCode\n
             * Returns the value stored in m_errorCode.
             * The value stored in m_errorCode will always be the resulting status of the most recent failing function call.
             * @return m_errorCode
             */
            star_knight::SKWindow::SKWindowErrCodes getErrorCode();

            /** getErrorMessage\n
             * Returns the value stored in m_errorMessage.
             * The value stored in m_errorCode will always be the resulting message of the most recent failing function call.
             * @return m_errorMessage.
             */
            std::string getErrorMessage();

            /** initSDLWindow\n
             * Used to initialize the main SDL_Window object and store it in the passed in pointer.
             * @param pwindow A reference to a pointer to hold the newly created window.
             * @return Status of function call. 0 if successful, 1 if failure.
             */
            void initSDLWindow();
        private:
            SDL_Window* m_pwindow;
            star_knight::SKWindow::SKWindowErrCodes m_errorCode;
            std::string m_errorMessage;

            /** initSDL\n
             * Specifically used to initialize the SDL system and the necessary subsystems (audio, video, etc.).
             * @return Status of function call. 0 if successful, 1 if failure.
             */
            void initSDL();

            /** destroySDL\n
             * Destroys various SDL objects (SDL_Window if not NULL) and the SDL system and its subsystems (video, audio, etc.).
             * @param pwindow Window to be destructed if not NULL.
             */
            void destroySDL();

             /** saveError\n
              * Saves error status and message.
              * If any of the functions in this class encounter an error, this is called to set the specific message and the errorCode variable.
              * @param prependedToError String to prepend to the SDL error message. Expected to be \n and null-terminated.
              * @param errorCode Error code to save. Expected to be one of SKWindowErrCodes.
              */
            void saveError(const std::string& prependedToError, SKWindowErrCodes errorCode);
    };
} // star_knight

#endif //STAR_KNIGHT_WINDOW_AND_INPUT_H
