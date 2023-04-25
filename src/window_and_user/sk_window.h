// Created on: 24/04/23.
// Author: DendyA
//

#ifndef STAR_KNIGHT_WINDOW_AND_INPUT_H
#define STAR_KNIGHT_WINDOW_AND_INPUT_H

#include <string>

#include "SDL.h"

namespace star_knight
{
    class SKWindow final
    {
        public:
            SKWindow();
            ~SKWindow();

            SDL_Window* getpwindow();
            int getIsError();
            std::string getErrorMessage();

            void initSDLWindow();
        private:
            SDL_Window* m_pwindow;
            int m_isError; // TODO(DendyA): I want to make this an error enum.
            std::string m_errorMessage;

            void initSDL();
            void destroySDL();
            void saveError(const std::string& prependedToError);
    };
} // star_knight

#endif //STAR_KNIGHT_WINDOW_AND_INPUT_H
