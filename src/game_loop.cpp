// Created on: 01/05/23.
// Author: DendyA

#include <iostream>

#include "bgfx.h"
// IMPORTANT: This MUST be included before SDL_syswm.h since in that header None is defined as 0L which breaks bx/math.h
// Moving it here, bx/math.h defines the None tag instead and SDL_syswm.h has an ifndef guard to check if None is not defined.
#include "bx/math.h"

#include "SDL_events.h"

#include "sk_global_defines.h"

#include "window_and_user/sk_window.h"
#include "renderer/initializer.h"
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

                const bx::Vec3 at  = { 0.0f, 0.0f,   0.0f };
                const bx::Vec3 eye = { 0.0f, 0.0f, 10.0f };

//             Set view and projection matrix for view 0.
                float view[16];
                bx::mtxLookAt(view, eye, at);

                float proj[16];
                bx::mtxProj(proj,
                            60.0f,
                            float(STARTING_SCREEN_WIDTH) / float(STARTING_SCREEN_HEIGHT),
                            0.1f, 100.0f,
                            bgfx::getCaps()->homogeneousDepth);

                bgfx::setViewTransform(0, view, proj);

                // Set view 0 default viewport.
                bgfx::setViewRect(0, 0, 0,
                                  (uint16_t)STARTING_SCREEN_WIDTH,
                                  (uint16_t)STARTING_SCREEN_HEIGHT);

                bgfx::touch(0);


                float mtx[16];
                bx::mtxRotateY(mtx, 0.0f);

                // position x,y,z
                mtx[12] = 0.0f;
                mtx[13] = 0.0f;
                mtx[14] = 0.0f;

                // Set model matrix for rendering.
                bgfx::setTransform(mtx);

                // Set vertex and index buffer.
                bgfx::setVertexBuffer(0, vertexBufferHandle);
                bgfx::setIndexBuffer(indexBufferHandle);

                // Set render states.
                bgfx::setState(BGFX_STATE_DEFAULT);

                // Submit primitive for rendering to view 0.
                bgfx::submit(0, programHandle);

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