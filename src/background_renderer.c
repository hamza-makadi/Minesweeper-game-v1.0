#include "../include/struct.h"
#include "../include/background_renderer.h"
#include <math.h>

void renderBackground(SDL_Surface *screen, SDL_Surface *stars, SDL_Surface *numbers) {

    SDL_Rect starPosition= {frameTimer%2200, 0};
    SDL_BlitSurface(stars, NULL, screen, &starPosition);
    SDL_Rect starPosition2= {(frameTimer%2200)-2200, 0};
    SDL_BlitSurface(stars, NULL, screen, &starPosition2);

    SDL_Rect numberPosition= {((int)(frameTimer*1.5)%2200), 0};
    SDL_BlitSurface(numbers, NULL, screen, &numberPosition);
    SDL_Rect numberPosition2= {(((int)(frameTimer*1.5)%2200)-2200), 0};
    SDL_BlitSurface(numbers, NULL, screen, &numberPosition2);
}

void freeBackground(SDL_Surface *stars, SDL_Surface *numbers){
    SDL_FreeSurface(numbers);
    SDL_FreeSurface(stars);
}


