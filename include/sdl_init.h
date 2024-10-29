#ifndef SDLINIT_H
#define SDLINIT_H

#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL_image.h>
#include "struct.h"

// Function to initialize SDL and set up the window
void initialize_sdl(SDL_Surface **window);

// Function to initialize SDL font using the SDL_ttf library and clean it
void cleanupFonts();
void initializeFonts(const char *fontPath) ;

// Function to handle window resizing
void resize_window(SDL_Surface **window, SDL_Event event);

#endif


