#include <iostream>

#include "SDL2/SDL.h"
#include "SDL2/SDL_syswm.h"

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
 * @param pwindow Pointer to hold the newly created window.
 * @return Status of function call. 0 if successful, 1 if failure.
 */
int initSDLWindow(SDL_Window* pwindow)
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

int main(int argc, char* args[])
{
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
        }
    }

    destroySDL(window);
	return exitCondition;
}
