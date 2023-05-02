// Created on: 24/04/23.
// Author: DendyA

#include <iostream>

#include "sk_global_defines.h"

#include "sk_window.h"

namespace star_knight
{
    star_knight::SKWindow::SKWindow()
    {
        m_errorCode = kNoErr;
        m_pwindow = nullptr;
        m_errorMessage = "";

        initSDL();
    }

    star_knight::SKWindow::~SKWindow()
    {
        destroySDL();
    }

    SDL_Window*
    SKWindow::getpwindow()
    {
        return m_pwindow;
    }

    star_knight::SKWindow::SKWindowErrCodes
    SKWindow::getErrorCode()
    {
        return m_errorCode;
    }

    std::string
    SKWindow::getErrorMessage()
    {
        return m_errorMessage;
    }

    void
    star_knight::SKWindow::initSDL()
    {
        const uint32_t initFlags = SDL_INIT_VIDEO; // Any other flags needed can be OR'd together here.

        if(SDL_Init(initFlags) < 0)
        {
            saveError("Window was unable to be created!\n", kSDLInitErr);
        }
    }

    void
    star_knight::SKWindow::initSDLWindow()
    {
        static const int STARTING_WINDOW_POS_X = SDL_WINDOWPOS_CENTERED;
        static const int STARTING_WINDOW_POS_Y = SDL_WINDOWPOS_CENTERED;
        static const int WINDOW_CREATION_FLAGS = SDL_WINDOW_OPENGL; // Other flags can be OR'd together here.
        static const std::string WINDOW_CREATION_TITLE = "Star Knight";

        m_pwindow = SDL_CreateWindow(WINDOW_CREATION_TITLE.c_str(),
                                     STARTING_WINDOW_POS_X,
                                     STARTING_WINDOW_POS_Y,
                                     (int)STARTING_SCREEN_WIDTH,
                                     (int)STARTING_SCREEN_HEIGHT,
                                     WINDOW_CREATION_FLAGS);

        if(!m_pwindow)
        {
            saveError("Window was unable to be created!\n", kSDLWinCreateErr);
        }
    }

    void
    star_knight::SKWindow::destroySDL()
    {
        // Conditional since technically it returns immediately and sets SDL_GetError if passed a NULL window.
        if(m_pwindow)
        {
            SDL_DestroyWindow(m_pwindow);
        }

        //Quit SDL subsystems
        SDL_Quit();
    }

    void
    star_knight::SKWindow::saveError(const std::string& prependedToError, SKWindowErrCodes errorCode)
    {
        // String initialization since SDL_GetError() returns a char*
        m_errorMessage = prependedToError + "SDL Error: " + std::string(SDL_GetError());
        m_errorCode = errorCode;
    }
} // star_knight