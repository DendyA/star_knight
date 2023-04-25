// Created on: 26/03/23
// Author: DendyA

#include <iostream>

// IMPORTANT: This MUST be included before SDL_syswm.h since in that header None is defined as 0L which breaks bx/math.h
// Moving it here, bx/math.h defines the None tag instead and SDL_syswm.h has an ifndef guard to check if None is not defined.
#include "bx/math.h"

#include "SDL_syswm.h"

#include "bgfx/bgfx.h"
#include "bgfx/platform.h"


#include "window_and_user/sk_window.h"
#include "shaders/shader_manager.h"



/** Initializes various bgfx objects.
 * Initializes various bgfx objects like the Init struct, retrieves window information from SDL,
 * and initializes the bgfx system as a whole etc.
 * @param pwindow Pointer to the window bgfx is to render in.
 * @return Status of the function call. 0 if successful, 1 if failure.
 */
int initbgfx(SDL_Window* pwindow)
{
    int exitCondition = EXIT_SUCCESS;

    SDL_SysWMinfo windowManagementInfo;
    SDL_VERSION(&windowManagementInfo.version);

    if(SDL_GetWindowWMInfo(pwindow, &windowManagementInfo) == SDL_FALSE)
    {
        std::cerr << "Unable to get window management information: " << SDL_GetError() << std::endl;
        exitCondition = EXIT_FAILURE;
        return exitCondition;
    }

    bgfx::Init initData;

//    FIXME(DendyA): Technically, these two are also platform-specific. Need to make them platform-agnostic in the future.
    initData.type = bgfx::RendererType::OpenGL;
    initData.vendorId = BGFX_PCI_ID_NVIDIA;

//    FIXME(DendyA): Technically this is platform-specific. Make it platform-agnostic in the future.
//      In particular, if Ubuntu uses Wayland, this will (obviously) crash given that X isn't being used.
    initData.platformData.ndt = windowManagementInfo.info.x11.display;
    initData.platformData.nwh = (void*)(uintptr_t)windowManagementInfo.info.x11.window;

    bgfx::renderFrame(); // This makes the program run in single threaded mode. Removing this call makes it multithreaded.

    if(!bgfx::init(initData))
    {
        std::cerr << "Unable to initialize bgfx!" << std::endl;
        exitCondition = EXIT_FAILURE;
    }

    return exitCondition;
}

/*** Initializes bgfx view objects.
 * Specifically used to set debug flags, reset the backbuffer size, and inits view objects among other things.
 */
void initbgfxView()
{
    // Consts to reset screen size. TODO(DendyA): These need to be kept in a more globally accessible location.
    const uint32_t SCREEN_WIDTH = 1280u;
    const uint32_t SCREEN_HEIGHT = 1024u;

    bgfx::reset(SCREEN_WIDTH, SCREEN_HEIGHT, BGFX_RESET_VSYNC);

    bgfx::setDebug(BGFX_DEBUG_TEXT);

    bgfx::setViewRect(0, 0, 0, (uint16_t)SCREEN_WIDTH, (uint16_t)SCREEN_HEIGHT);

    bgfx::setViewClear(0, BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH, 0x443355FF, 1.0f, 0);

    bgfx::touch(0);
}

/** Destroys the bgfx system as a whole.
 *  Destroys the bgfx system as a whole. bgfx MUST be initialized before shutdown is called. Otherwise a fatal error occurs.
 */
void destroybgfx()
{
    bgfx::shutdown();
}

int main(int argc, char* args[])
{
    // Consts to set view and projection matrix. TODO(DendyA): These need to be kept in a more globally accessible location.
    const uint32_t SCREEN_WIDTH = 1280u;
    const uint32_t SCREEN_HEIGHT = 1024u;

    int exitCondition = EXIT_SUCCESS;

//    Since the SDL_Quit function is called in SKWindow's destructor, simply terminating the program will handle the shutdown of the SDL subsytems.
    star_knight::SKWindow skWindow = star_knight::SKWindow();

//    This errors-out and returns immediately since there is not a ton to be done if SDL can't initialize.
    if(skWindow.getIsError())
    {
        std::cerr << skWindow.getErrorMessage() << std::endl;
        return skWindow.getIsError();
    }

    skWindow.initSDLWindow();

//    This errors-out and returns immediately since have a window fail to initialize means no game to display.
    if(skWindow.getIsError())
    {
        std::cerr << skWindow.getErrorMessage() << std::endl;
        return skWindow.getIsError();
    }

    exitCondition = initbgfx(skWindow.getpwindow());

//    This errors-out and returns immediately since having no bgfx corresponds to the inability to display graphics.
    if(exitCondition != EXIT_SUCCESS)
    {
//      This does not destroy bgfx here because neither of the failure paths in the initbgfx function will result in
//      a successful initialization of bgfx. And bgfx errors-out if it is attempted to be destroyed while uninitialized.
        return exitCondition;
    }

    initbgfxView();

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
                        float(SCREEN_WIDTH)/float(SCREEN_HEIGHT),
                        0.1f, 100.0f,
                        bgfx::getCaps()->homogeneousDepth);

            bgfx::setViewTransform(0, view, proj);

            // Set view 0 default viewport.
            bgfx::setViewRect(0, 0, 0,
                              SCREEN_WIDTH,
                              SCREEN_HEIGHT);

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

    destroybgfx();

	return exitCondition;
}
