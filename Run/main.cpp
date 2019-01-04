#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>


int main(int argc, char* argv[])
{
    
    SDL_Init(SDL_INIT_EVERYTHING);

    const int imgFlags = IMG_INIT_PNG;
    if (!(IMG_Init(imgFlags) & imgFlags)) {
        std::cout << "SDL_image could not initialize! SDL_image Error: " << IMG_GetError() << '\n';
        return 1;
    }


    SDL_Window *window = SDL_CreateWindow(
        "Game",
        SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED,
        480,
        432,
        SDL_WINDOW_SHOWN
    );

    if (!window) {
        std::cout << "Couldn't create window! SDL Error: " << SDL_GetError() << '\n';
        return 1;
    }

    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    
    if (!renderer) {
        std::cout << "Renderer could not be created! SDL Error: " << SDL_GetError() << '\n';
        return 1;
    }

    //SDL_RenderSetScale(renderer, 3.0f, 3.0f);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);


    bool is_running = true;
    SDL_Event event;

    while (is_running)
    {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                is_running = false;
            }
        }



        SDL_RenderClear(renderer);
        SDL_RenderPresent(renderer);

        SDL_Delay(250);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);

    IMG_Quit();
    SDL_Quit();

    return 0;
}
