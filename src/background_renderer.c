#include "../include/struct.h"
#include "../include/background_renderer.h"
#include <math.h>

/**
 * Renders a scrolling background on the screen, composed of two layers: stars and numbers.
 * Creates a parallax effect by moving the two layers at different speeds, using `frameTimer`
 * as a reference for their positions. The function draws two instances of each layer to
 * achieve seamless scrolling when the images reset to the starting position.
 *
 * Parameters:
 *   - SDL_Surface *screen: The main surface where all visuals are rendered.
 *   - SDL_Surface *stars: Surface containing the stars layer of the background.
 *   - SDL_Surface *numbers: Surface containing the numbers layer of the background.
 */
void renderBackground(SDL_Surface *screen, SDL_Surface *stars, SDL_Surface *numbers) {

    // Define the position of the first stars layer, moving horizontally based on frameTimer
    SDL_Rect starPosition = {frameTimer % 2200, 0};
    SDL_BlitSurface(stars, NULL, screen, &starPosition); // Render the stars layer at calculated position

    // Define the position of the second stars layer, positioned for seamless looping
    SDL_Rect starPosition2 = {(frameTimer % 2200) - 2200, 0};
    SDL_BlitSurface(stars, NULL, screen, &starPosition2); // Render the stars layer offset by its width

    // Define the position of the first numbers layer, moving faster than the stars layer for parallax effect
    SDL_Rect numberPosition = {(int)(frameTimer * 1.5) % 2200, 0};
    SDL_BlitSurface(numbers, NULL, screen, &numberPosition); // Render the numbers layer at calculated position

    // Define the position of the second numbers layer, positioned for seamless looping
    SDL_Rect numberPosition2 = {((int)(frameTimer * 1.5) % 2200) - 2200, 0};
    SDL_BlitSurface(numbers, NULL, screen, &numberPosition2); // Render the numbers layer offset by its width
}


/**
 * Frees the memory associated with the background layers.
 * This function releases the SDL surfaces for the stars and numbers
 * layers to prevent memory leaks when they are no longer needed.
 *
 * Parameters:
 *   - SDL_Surface *stars: Surface containing the stars layer of the background.
 *   - SDL_Surface *numbers: Surface containing the numbers layer of the background.
 */
void freeBackground(SDL_Surface *stars, SDL_Surface *numbers) {

    // Free the surface memory allocated for the numbers layer
    SDL_FreeSurface(numbers);

    // Free the surface memory allocated for the stars layer
    SDL_FreeSurface(stars);
}



