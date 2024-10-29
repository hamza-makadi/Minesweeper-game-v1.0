#include "../include/sdl_init.h"
#include "../include/struct.h"
#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL_image.h>

// Function to initialize SDL and set up the window
void initialize_sdl(SDL_Surface **window) {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        fprintf(stderr, "Unable to initialize SDL: %s\n", SDL_GetError());
        GameState = 0;
        return;
    }

    /************************************

    I was trying to debug my code using the printf function but nothing was showing up in the console
    so i did a quick search and i found the solution on this form
    https://forums.codeblocks.org/index.php?topic=4887.0

    *************************************/

    freopen("CON", "w", stdout); // redirects stdout
    freopen("CON", "w", stderr); // redirects stderr

    /************************************/

    *window = SDL_SetVideoMode(screenWidth, screenHeight, 32, SDL_SWSURFACE | SDL_RESIZABLE);
    if (!(*window)) {
        fprintf(stderr, "Unable to set video mode: %s\n", SDL_GetError());
        SDL_Quit();
        GameState = 0;
        return;
    }

    SDL_WM_SetCaption("Minesweeper-1.0", NULL);

}

void cleanupFonts() {
    int i;
    for (i = 0; i < NUM_FONTS; i++) {
        if (fonts[i]) {
            TTF_CloseFont(fonts[i]);
            fonts[i] = NULL;
        }
    }
}

void initializeFonts(const char *fontPath) {
    if (TTF_Init() == -1) {
        printf("SDL_ttf could not initialize! SDL_ttf Error: %s\n", TTF_GetError());
        GameState = 0;
    }
    int fontSizes[NUM_FONTS] = {12, 16, 24, 32, 48}; // different sizes of the font
    int i;
    for (i = 0; i < NUM_FONTS; i++) {
        fonts[i] = TTF_OpenFont(fontPath, fontSizes[i]);
        if (!fonts[i]) {
            fprintf(stderr, "Failed to load font at size %d: %s\n", fontSizes[i], TTF_GetError());
            cleanupFonts();
            return;
        }
    }
}

// Function to handle window resizing
void resize_window(SDL_Surface **window, SDL_Event event){
    screenWidth = event.resize.w;
    screenHeight = event.resize.h;
    *window = SDL_SetVideoMode(screenWidth, screenHeight, 32, SDL_SWSURFACE | SDL_RESIZABLE);
    if (!(*window)) {
        fprintf(stderr, "Unable to resize window: %s\n", SDL_GetError());
        return;
    }
}



