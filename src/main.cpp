#include <SDL2/SDL.h>
#include <iostream>

// Test code to get a simple screen with a white square to appear.
int main( int argc, char* args[] )
{
    // Consts to init screen size.
    const int SCREEN_WIDTH = 1280;
    const int SCREEN_HEIGHT = 1024;

    int exitCondition = EXIT_SUCCESS;

	SDL_Window* window;
	SDL_Surface* screenSurface;

	if(SDL_Init(SDL_INIT_VIDEO) < 0)
	{
        std::cout << "SDL was unable to initialize! SDL Error: " << SDL_GetError() << std::endl;
        exitCondition = EXIT_FAILURE;
	}
	else
	{
		window = SDL_CreateWindow("Star Knight", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_OPENGL);

		if(!window)
		{
            std::cout << "Window was unable to be created! SDL Error: " << SDL_GetError() << std::endl;
            exitCondition = EXIT_FAILURE;
		}
		else
		{
			screenSurface = SDL_GetWindowSurface(window);

			// Fills a rectangle, drawn on the screen surface, and fills it all white.
			SDL_FillRect(screenSurface, nullptr, SDL_MapRGB(screenSurface->format, 0xFF, 0xFF, 0xFF));

			SDL_UpdateWindowSurface(window);

            //TODO(DendyA): Need to remove this hack once better event handling is implemented.
            // Main event loop for keeping the window up until the user quits the window.
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
            // Moved this inside the else since technically it returns immediately and sets SDL_GetError if passed a NULL window.
            SDL_DestroyWindow(window);
		}
    }

	//Quit SDL subsystems
	SDL_Quit();

	return exitCondition;
}
