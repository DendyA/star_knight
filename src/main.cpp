// Created on: 26/03/23
// Author: DendyA

#include <iostream>

// IMPORTANT: This MUST be included before SDL_syswm.h since in that header None is defined as 0L which breaks bx/math.h
// Moving it here, bx/math.h defines the None tag instead and SDL_syswm.h has an ifndef guard to check if None is not defined.
#include "bx/math.h"

#include "SDL.h"
#include "SDL_syswm.h"

#include "bgfx/bgfx.h"
#include "bgfx/platform.h"

#include "shaders/shader_manager.h"

/*** Initialize SDL and its various subsystems.
 * Specifically used to initialize the SDL system and the necessary subsystems (audio, video, etc.).
 * @return Status of function call. 0 if successful, 1 if failure.
 */
int initSDL()
{
    const uint32_t initFlags = SDL_INIT_VIDEO; // Any other flags needed can be OR'd together here.
    int exitCondition = EXIT_SUCCESS;

    if(SDL_Init(initFlags) < 0)
    {
        std::cerr << "SDL was unable to initialize! SDL Error: " << SDL_GetError() << std::endl;
        exitCondition = EXIT_FAILURE;
    }

    return exitCondition;
}

/*** Initialize and create the SDL_Window object.
 * Used to initialize the main SDL_Window object and store it in the passed in pointer.
 * @todo Parameterize the various window creation parameters. (x, y, size, title, and flags).
 * @param pwindow A reference to a pointer to hold the newly created window.
 * @return Status of function call. 0 if successful, 1 if failure.
 */
int initSDLWindow(SDL_Window*& pwindow)
{
    // Consts to init screen size.
    const int SCREEN_WIDTH = 1280;
    const int SCREEN_HEIGHT = 1024;

    int exitCondition = EXIT_SUCCESS;

    pwindow = SDL_CreateWindow("Star Knight", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_OPENGL);

    if(!pwindow)
    {
        std::cout << "Window was unable to be created! SDL Error: " << SDL_GetError() << std::endl;
        exitCondition = EXIT_FAILURE;
    }

    return exitCondition;
}

/*** Used to destroy various SDL objects and the SDL system as a whole.
 * Destroys various SDL objects (SDL_Window if not NULL) and the SDL system and its subsystems (video, audio, etc.).
 * @param pwindow Window to be destructed if not NULL.
 */
void destroySDL(SDL_Window* pwindow)
{
    // Conditional since technically it returns immediately and sets SDL_GetError if passed a NULL window.
    if(pwindow)
    {
        SDL_DestroyWindow(pwindow);
    }

    //Quit SDL subsystems
    SDL_Quit();
}

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

    SDL_Window* window;

    exitCondition = initSDL();

    if(exitCondition != EXIT_SUCCESS)
    {
        destroySDL(window);
        return exitCondition;
    }

    exitCondition = initSDLWindow(window);

    if(exitCondition != EXIT_SUCCESS)
    {
        destroySDL(window);
        return exitCondition;
    }

    exitCondition = initbgfx(window);

    if(exitCondition != EXIT_SUCCESS)
    {
//      This does not destroy bgfx here because neither of the failure paths in the initbgfx function will result in
//      a successful initialization of bgfx. And bgfx errors-out if it is attempted to be destroyed while uninitialized.
        destroySDL(window);
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

    destroybgfx();
    destroySDL(window);
	return exitCondition;
}
