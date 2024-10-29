#ifndef SCREENMANAGER_H
#define SCREENMANAGER_H

#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL_image.h>
#include "struct.h"

// Function to initialize main menu screen
Screen* createMenuScreen();

// Function to initialize mode screen
Screen* createModeScreen();

// Function to render any screen
void renderScreen(Screen *Screen, SDL_Surface *screen);

void freeScreen(Screen *screen);

#endif


