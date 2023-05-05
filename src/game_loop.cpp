// Created on: 01/05/23.
// Author: DendyA

#include <iostream>

#include "bgfx.h"

#include "SDL_events.h"

#include "renderer/transformation_manager.h"
#include "shaders/shader_manager.h"

#include "game_loop.h"

namespace star_knight
{
    star_knight::GameLoop::GameLoop()
    {
        m_errorCode = kNoErr;
        m_errorMessage = "";

        initializeSDLGameObjects();
        initializebgfxGameObjects();
    }

    star_knight::GameLoop::~GameLoop()
    {
        // When destroying GameLoop, we only want to call destorybgfx if the bgfxInitializer initialized properly because otherwise a fatal error occurs.
        // These two error codes are the only ones that could be thrown before bgfx would initialize. Meaning as long as
        // it isn't reporting these two error codes, then bgfx is safe to destroy.
        if(m_bgfxInitializer.getErrorCode() != star_knight::Initializer::SKRendererInitErrCodes::kSDLNoManagementWindowInfoErr ||
            m_bgfxInitializer.getErrorCode() != star_knight::Initializer::SKRendererInitErrCodes::kbgfxInitErr)
        {
            m_bgfxInitializer.destroybgfx();
        }
    }

    GameLoop::SKGameLoopErrCodes
    GameLoop::getErrorCode()
    {
        return m_errorCode;
    }

    std::string
    GameLoop::getErrorMessage()
    {
        return m_errorMessage;
    }

    void
    star_knight::GameLoop::initializeSDLGameObjects()
    {
//    Since the SDL_Quit function is called in SKWindow's destructor, simply terminating the program will handle the shutdown of the SDL subsytems.
        m_skWindow = star_knight::SKWindow();

//    This errors-out and returns immediately since there is not a ton to be done if SDL can't initialize.
        if(m_skWindow.getErrorCode() != star_knight::SKWindow::SKWindowErrCodes::kNoErr)
        {
            saveError(m_skWindow.getErrorMessage(), kSDLGameObjectsInitErr);
            return;
        }

        m_skWindow.initSDLWindow();

//    This errors-out and returns immediately since having a window fail to initialize means no game to display.
        if(m_skWindow.getErrorCode() != star_knight::SKWindow::SKWindowErrCodes::kNoErr)
        {
            saveError(m_skWindow.getErrorMessage(), kSDLGameObjectsInitErr);
            return;
        }
    }

    void
    star_knight::GameLoop::initializebgfxGameObjects()
    {
        // Not setting error here since the issue was due to an SDL init error.
        if(m_errorCode != kNoErr)
        {
            return;
        }

        m_bgfxInitializer = star_knight::Initializer(m_skWindow.getpwindow());

//    This errors-out and returns immediately since having no bgfx corresponds to the inability to display graphics.
        if(m_bgfxInitializer.getErrorCode() != star_knight::Initializer::SKRendererInitErrCodes::kNoErr)
        {
            saveError(m_bgfxInitializer.getErrorMessage(), kbgfxGameObjectsInitErr);
            return;
        }

        m_bgfxInitializer.initbgfxView();
    }

    GameLoop::SKGameLoopErrCodes
    star_knight::GameLoop::mainLoop()
    {

        bgfx::VertexBufferHandle vertexBufferHandle = star_knight::ShaderManager::initVertexBuffer();
        bgfx::IndexBufferHandle indexBufferHandle = star_knight::ShaderManager::initIndexBuffer();
        bgfx::ProgramHandle programHandle = star_knight::ShaderManager::generateProgram("../compiled_shaders/vertex/vs_simple.bin",
                                                                           "../compiled_shaders/fragment/fs_simple.bin");

        star_knight::TransformationManager transformManager = star_knight::TransformationManager();

        SDL_Event currEvent;
        bool quit = false;

        while(!quit)
        {
            while(SDL_PollEvent(&currEvent))
            {
                if(currEvent.type == SDL_QUIT)
                {
                    quit = true;
                }

                // Set vertex and index buffer.
                bgfx::setVertexBuffer(0, vertexBufferHandle);
                bgfx::setIndexBuffer(indexBufferHandle);

                // Set render states.
                bgfx::setState(BGFX_STATE_DEFAULT);

                // Submit primitive for rendering to view 0.
                bgfx::submit(0, programHandle);

                transformManager.updateViewTransform(0);
                transformManager.setTransformMatrix();

                bgfx::frame();
            }
        }

        bgfx::destroy(vertexBufferHandle);
        bgfx::destroy(indexBufferHandle);
        bgfx::destroy(programHandle);

        return kNoErr;
    }

    void
    star_knight::GameLoop::saveError(const std::string &errorMessage, SKGameLoopErrCodes errorCode)
    {
        m_errorMessage = errorMessage;
        m_errorCode = errorCode;
    }

} // star_knight