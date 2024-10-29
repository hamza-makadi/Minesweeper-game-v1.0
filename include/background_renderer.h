#ifndef BACKGROUNDRENDERER_H
#define BACKGROUNDRENDERER_H

#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL_image.h>

void renderBackground(SDL_Surface *screen, SDL_Surface *stars, SDL_Surface *numbers);
void freeBackground(SDL_Surface *stars, SDL_Surface *numbers);

#endif



