// Created on: 01/05/23.
// Author: DendyA

#include <iostream>

#include "bgfx.h"

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

    void
    GameLoop::handleKeyDownEvent(SDL_Event keyDownEvent)
    {
        switch(keyDownEvent.key.keysym.sym)
        {
            case SDLK_LEFT:
                m_transformManager.view_translateX(0.1);
                break;
            case SDLK_RIGHT:
                m_transformManager.view_translateX(-0.1);
                break;
            case SDLK_UP:
                m_transformManager.view_translateY(0.1);
                break;
            case SDLK_DOWN:
                m_transformManager.view_translateY(-0.1);
                break;
            default:
                break;
        }
    }

    GameLoop::SKGameLoopErrCodes
    star_knight::GameLoop::mainLoop()
    {

        bgfx::VertexBufferHandle vertexBufferHandle = star_knight::ShaderManager::initVertexBuffer();
        bgfx::IndexBufferHandle indexBufferHandle = star_knight::ShaderManager::initIndexBuffer();

        bgfx::ProgramHandle programHandle{};

        bool generateProgramStatus = star_knight::ShaderManager::generateProgram("vs_simple.bin","fs_simple.bin", programHandle);

        if(!generateProgramStatus)
        {
            saveError("GameLoop: Error while trying to generate shader program\n", kShaderManagerProgramGenerateErr);

            // TODO(DendyA): If possible, make these handles member variables in this class and then these can be destroyed from the destructor.
            bgfx::destroy(vertexBufferHandle);
            bgfx::destroy(indexBufferHandle);

            return kShaderManagerProgramGenerateErr;
        }

        m_transformManager = star_knight::TransformationManager();

        m_transformManager.setTransformMatrix();

        SDL_Event currEvent;
        bool quit = false;

        while(!quit)
        {
            while(SDL_PollEvent(&currEvent))
            {
                switch(currEvent.type)
                {
                    case SDL_QUIT:
                        quit = true;
                        break;
                    case SDL_KEYDOWN:
                        handleKeyDownEvent(currEvent);
                        break;
                    default:
                        break;
                }

                // Set vertex and index buffer.
                bgfx::setVertexBuffer(0, vertexBufferHandle);
                bgfx::setIndexBuffer(indexBufferHandle);

                // Set render states.
                bgfx::setState(BGFX_STATE_DEFAULT);

                // Submit primitive for rendering to view 0.
                // FIXME(DendyA): When this is put into the main game loop, this causes a delay in closing the game window.
                //  Related to issue #17.
                bgfx::submit(0, programHandle);

                m_transformManager.updateViewTransform(0);

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