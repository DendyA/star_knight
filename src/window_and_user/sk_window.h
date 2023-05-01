// Created on: 24/04/23.
// Author: DendyA

#ifndef STAR_KNIGHT_WINDOW_AND_INPUT_H
#define STAR_KNIGHT_WINDOW_AND_INPUT_H

#include <string>

#include "SDL.h"

namespace star_knight
{
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

            SKWindow();
            ~SKWindow();

            SDL_Window* getpwindow();
            star_knight::SKWindow::SKWindowErrCodes getErrorCode();
            std::string getErrorMessage();

            void initSDLWindow();
        private:
            SDL_Window* m_pwindow;
            star_knight::SKWindow::SKWindowErrCodes m_errorCode;
            std::string m_errorMessage;

            void initSDL();
            void destroySDL();
            void saveError(const std::string& prependedToError, SKWindowErrCodes errorCode);
    };
} // star_knight

#endif //STAR_KNIGHT_WINDOW_AND_INPUT_H
