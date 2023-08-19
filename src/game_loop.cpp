// Created on: 01/05/23.
// Author: DendyA

#include <iostream>

#include "bgfx/bgfx.h"

#include "models/sk_parser.h"
#include "models/sk_mesh.h"

#include "game_loop.h"

star_knight::GameLoop::GameLoop() :
    m_shaderManager(),
    m_transformManager()
{
    m_errorCode = kNoErr;
    m_errorMessage = "";

    initializeSDLGameObjects();
    initializebgfxGameObjects();
}

star_knight::GameLoop::~GameLoop()
{
    // This MUST be called rather than waiting for m_shaderManager's destructor to be called since the destructor is called
    // after the call to destroybgfx(). Meaning that the program handle is left dangling and bgfx will report a memory leak.
    m_shaderManager.destroyProgramHandle();

    // When destroying GameLoop, we only want to call destorybgfx if the bgfxInitializer initialized properly because otherwise a fatal error occurs.
    // These two error codes are the only ones that could be thrown before bgfx would initialize. Meaning as long as
    // it isn't reporting these two error codes, then bgfx is safe to destroy.
    if(m_bgfxInitializer.getErrorCode() != star_knight::Initializer::SKRendererInitErrCodes::kSDLNoManagementWindowInfoErr ||
        m_bgfxInitializer.getErrorCode() != star_knight::Initializer::SKRendererInitErrCodes::kbgfxInitErr)
    {
        m_bgfxInitializer.destroybgfx();
    }
}

star_knight::GameLoop::SKGameLoopErrCodes
star_knight::GameLoop::getErrorCode()
{
    return m_errorCode;
}

std::string
star_knight::GameLoop::getErrorMessage()
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
        // destroybgfx() (on m_bgfxInitializer) is never called here because the only way an error is caught here,
        // is before bgfx is initialized. And calling destorybgfx() before bgfx is initialized causes a fatal error.
        saveError(m_bgfxInitializer.getErrorMessage(), kbgfxGameObjectsInitErr);
        return;
    }

    m_bgfxInitializer.initbgfxView();
}

void
star_knight::GameLoop::handleKeyDownEvent(SDL_Event keyDownEvent)
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

star_knight::GameLoop::SKGameLoopErrCodes
star_knight::GameLoop::mainLoop()
{
    star_knight::GameLoop::SKGameLoopErrCodes gameLoopResult = kNoErr;

    std::unique_ptr<SKMesh> mesh = std::unique_ptr<SKMesh>(nullptr);

    bool openMeshStatus = star_knight::SKParser::openMeshFile("../lib/bgfx_cmake/bgfx/examples/runtime/meshes/bunny.bin", mesh);

    if(!openMeshStatus)
    {
        saveError("GameLoop: Error while trying to open mesh file\n", kOpenMeshFileErr);
        gameLoopResult = kOpenMeshFileErr;

        return gameLoopResult;
    }

    float mtx[16];
    bx::mtxRotateY(mtx, 0.0f);

    // position x,y,z
    mtx[12] = 0.0f;
    mtx[13] = 0.0f;
    mtx[14] = 0.0f;

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

            // Submit mesh for rendering to view 0.
            // FIXME(DendyA): When this is put into the main game loop, this causes a delay in closing the game window.
            //  Related to issue #17.
            m_shaderManager.update(0, mesh, mtx, 0xffffffffffffffffUL);

            if(m_shaderManager.getErrorCode() != star_knight::ShaderManager::kNoErr)
            {
                saveError("GameLoop: Error while trying to update ShaderManager\n", kShaderManagerUpdateErr);
                quit = true;
                gameLoopResult = kShaderManagerUpdateErr;

                continue;
            }

            m_transformManager.update(0);

            bgfx::frame();
        }
    }

    mesh->destroyHandles();

    return gameLoopResult;
}

void
star_knight::GameLoop::saveError(const std::string &errorMessage, SKGameLoopErrCodes errorCode)
{
    m_errorMessage = errorMessage;
    m_errorCode = errorCode;
}
