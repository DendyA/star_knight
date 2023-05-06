// Created on: 26/04/23.
// Author: DendyA

#include <iostream>

#include "SDL_syswm.h"

#include "bgfx/bgfx.h"
#include "bgfx/platform.h"

#include "sk_global_defines.h"

#include "initializer.h"

star_knight::Initializer::Initializer()
{
    m_errorCode = kNoErr;
    m_errorMessage = "";
}

star_knight::Initializer::Initializer(SDL_Window* pwindow)
{
    m_errorCode = kNoErr;
    m_errorMessage = "";

    // TODO(DendyA): If this window is used for more than just getting window info, make it a member variable. Probably want it to be a shared_ptr.
    initbgfx(pwindow);
}

star_knight::Initializer::~Initializer() = default;

star_knight::Initializer::SKRendererInitErrCodes
star_knight::Initializer::getErrorCode()
{
    return m_errorCode;
}

std::string
star_knight::Initializer::getErrorMessage()
{
    return m_errorMessage;
}

void
star_knight::Initializer::initbgfx(SDL_Window* pwindow)
{
    SDL_SysWMinfo windowManagementInfo;
    SDL_VERSION(&windowManagementInfo.version);

    if(SDL_GetWindowWMInfo(pwindow, &windowManagementInfo) == SDL_FALSE)
    {
        const std::string errorMessage = "Unable to get window management information.\nSDL Error:" + std::string(SDL_GetError());
        saveError(errorMessage, kSDLNoManagementWindowInfoErr);
        return;
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
        saveError("SK ~ BGFX Error: Unable to initialize bgfx!\n", kbgfxInitErr);
    }
}

void
star_knight::Initializer::initbgfxView()
{
    bgfx::reset(STARTING_SCREEN_WIDTH, STARTING_SCREEN_HEIGHT, BGFX_RESET_VSYNC);

    bgfx::setDebug(BGFX_DEBUG_TEXT);

    bgfx::setViewRect(0, 0, 0, (uint16_t)STARTING_SCREEN_WIDTH, (uint16_t)STARTING_SCREEN_HEIGHT);

    bgfx::setViewClear(0, BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH, 0x443355FF, 1.0f, 0);

    bgfx::touch(0);
}

void
star_knight::Initializer::destroybgfx()
{
    bgfx::shutdown();
}

void
star_knight::Initializer::saveError(const std::string &errorMessage, SKRendererInitErrCodes errorCode)
{
    m_errorMessage = errorMessage;
    m_errorCode = errorCode;
}
