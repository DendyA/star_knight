// Created on: 24/04/23.
// Author: DendyA

#include <iostream>

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
        // Consts to init screen size. // TODO(DendyA): Move to a more globally accessible location.
        const int SCREEN_WIDTH = 1280;
        const int SCREEN_HEIGHT = 1024;

        m_pwindow = SDL_CreateWindow("Star Knight", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_OPENGL);

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