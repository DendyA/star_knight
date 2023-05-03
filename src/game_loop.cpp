// Created on: 01/05/23.
// Author: DendyA

#include <iostream>

#include "bgfx.h"

#include "SDL_events.h"

#include "window_and_user/sk_window.h"
#include "renderer/initializer.h"
#include "renderer/transformation_manager.h"
#include "shaders/shader_manager.h"

#include "game_loop.h"

namespace star_knight
{
    star_knight::GameLoop::GameLoop() = default;
    star_knight::GameLoop::~GameLoop() = default;

    int
    star_knight::GameLoop::mainLoop()
    {

//    Since the SDL_Quit function is called in SKWindow's destructor, simply terminating the program will handle the shutdown of the SDL subsytems.
        star_knight::SKWindow skWindow = star_knight::SKWindow();

//    This errors-out and returns immediately since there is not a ton to be done if SDL can't initialize.
        if(skWindow.getErrorCode() != star_knight::SKWindow::SKWindowErrCodes::kNoErr)
        {
            std::cerr << skWindow.getErrorMessage() << std::endl;
            return skWindow.getErrorCode(); // Since the skWindowErrCodes enum is type defined as an int, this is valid.
        }

        skWindow.initSDLWindow();

//    This errors-out and returns immediately since having a window fail to initialize means no game to display.
        if(skWindow.getErrorCode() != star_knight::SKWindow::SKWindowErrCodes::kNoErr)
        {
            std::cerr << skWindow.getErrorMessage() << std::endl;
            return skWindow.getErrorCode(); // Since the skWindowErrCodes enum is type defined as an int, this is valid.
        }

        star_knight::Initializer bgfxInitalizer = star_knight::Initializer(skWindow.getpwindow());

//    This errors-out and returns immediately since having no bgfx corresponds to the inability to display graphics.
        if(bgfxInitalizer.getErrorCode() != star_knight::Initializer::SKRendererInitErrCodes::kNoErr)
        {
            std::cerr << bgfxInitalizer.getErrorMessage() << std::endl;

            if(bgfxInitalizer.getErrorCode() != star_knight::Initializer::SKRendererInitErrCodes::kbgfxInitErr)
            {
                // Only destory bgfx if bgfx was successfully initialized. If it wasn't, attempting to destroy it will cause a fatal error.
                bgfxInitalizer.destroybgfx();
            }

            return bgfxInitalizer.getErrorCode();
        }

        bgfxInitalizer.initbgfxView();

        bgfx::VertexBufferHandle vertexBufferHandle = ShaderManager::initVertexBuffer();
        bgfx::IndexBufferHandle indexBufferHandle = ShaderManager::initIndexBuffer();
        bgfx::ProgramHandle programHandle = ShaderManager::generateProgram("../compiled_shaders/vertex/vs_simple.bin",
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

        bgfxInitalizer.destroybgfx();

        return EXIT_SUCCESS;
    }
} // star_knight