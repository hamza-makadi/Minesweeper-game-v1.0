#include "../include/sdl_init.h"
#include "../include/struct.h"
#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL_image.h>
#include <SDL_mixer.h>

/**
 * Initializes SDL and sets up the window for the game.
 * This function initializes the SDL library, redirects stdout and stderr for debugging purposes,
 * and creates a window with the specified dimensions. It also sets the window caption.
 *
 * Parameters:
 *   - window: A pointer to an SDL_Surface pointer, which will hold the reference to the window surface.
 *
 * Note:
 * The function redirects the stdout and stderr to allow debugging via `printf` or `stderr` messages.
 */
void initialize_sdl(SDL_Surface **window) {
    // Initialize SDL Video subsystem
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        fprintf(stderr, "Unable to initialize SDL: %s\n", SDL_GetError());
        gameState = GAME_OFF;
        return;
    }

    if (SDL_Init(SDL_INIT_AUDIO) < 0) {
        printf("Failed to initialize SDL: %s\n", SDL_GetError());
        return;
    }

    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
        printf("SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError());
        return;
    }


    /************************************
    I was trying to debug my code using the printf function but nothing was showing up in the console.
    So I did a quick search and found the solution on this forum:
    https://forums.codeblocks.org/index.php?topic=4887.0
    *************************************/

    // Redirect stdout and stderr to the console to enable proper debugging output
    freopen("CON", "w", stdout);  // redirects stdout to console
    freopen("CON", "w", stderr);  // redirects stderr to console

    /************************************/

    // Set the video mode for the game window
    *window = SDL_SetVideoMode(screenWidth, screenHeight, 32, SDL_SWSURFACE | SDL_RESIZABLE);
    if (!(*window)) {
        fprintf(stderr, "Unable to set video mode: %s\n", SDL_GetError());
        SDL_Quit();
        gameState = GAME_OFF;
        return;
    }

    // Set the window title
    SDL_WM_SetCaption("Minesweeper-1.0", NULL);
}

/**
 * Handles resizing the SDL window when the user resizes the window.
 * The screen width and height are updated to the new dimensions provided by the `SDL_Event` resize event.
 * The window is then resized using `SDL_SetVideoMode` with the new dimensions.
 * If resizing fails, an error message is displayed and the game state is set to `GAME_OFF`.
 *
 * @param window A pointer to the SDL_Surface representing the window.
 * @param event An SDL event that contains the window resize information.
 */
void resize_window(SDL_Surface **window, SDL_Event event) {
    // Update the global screen dimensions based on the event's new width and height
    screenWidth = event.resize.w;
    screenHeight = event.resize.h;

    // Resize the SDL window with the new dimensions
    *window = SDL_SetVideoMode(screenWidth, screenHeight, 32, SDL_SWSURFACE | SDL_RESIZABLE);

    // Check if the window resize was successful
    if (!(*window)) {
        // If resizing fails, print an error and set the game state to GAME_OFF
        fprintf(stderr, "Unable to resize window: %s\n", SDL_GetError());
        gameState = GAME_OFF;
        return;
    }
}

/**
 * Initializes the fonts for the game by loading font files at various sizes.
 * This function uses the `TTF_OpenFont` function to open the font file at different sizes specified in the `fontSizes` array.
 * If any font fails to load, it will clean up previously loaded fonts and stop the initialization process.
 *
 * @param fontPath The path to the font file that should be used for the game.
 *
 * Note:
 * The `fonts` array should be defined globally with a size of `NUM_FONTS` to hold the font pointers.
 * `gameState` should be an integer representing the current state of the game.
 */
void initializeFonts(const char *fontPath) {
    // Initialize SDL_ttf library
    if (TTF_Init() == -1) {
        printf("SDL_ttf could not initialize! SDL_ttf Error: %s\n", TTF_GetError());
        gameState = GAME_OFF;  // Set the game state to OFF if initialization fails
        return;
    }

    int fontSizes[NUM_FONTS] = {12, 16, 24, 32, 48}; // Different font sizes
    int i;
    // Loop through the font sizes and load the font for each size
    for (i = 0; i < NUM_FONTS; i++) {
        fonts[i] = TTF_OpenFont(fontPath, fontSizes[i]);  // Open the font with the specified size
        if (!fonts[i]) {  // If the font fails to load, display an error message and clean up
            fprintf(stderr, "Failed to load font at size %d: %s\n", fontSizes[i], TTF_GetError());
            cleanupFonts();  // Clean up any previously loaded fonts
            return;
        }
    }
}

/**
 * Cleans up the fonts used in the game by closing and freeing the resources.
 * This function iterates over the fonts array, checking each element, and closes any valid font using `TTF_CloseFont`.
 * After closing a font, it sets the corresponding pointer to NULL to avoid dangling references.
 *
 * Note:
 * The `fonts` array should be defined globally with a size of `NUM_FONTS`, and each element of the array should hold a valid TTF_Font pointer.
 */
void cleanupFonts() {
    int i;
    // Iterate over all fonts
    for (i = 0; i < NUM_FONTS; i++) {
        // Check if the font is not NULL
        if (fonts[i]) {
            // Close the font to free its resources
            TTF_CloseFont(fonts[i]);
            fonts[i] = NULL;  // Set the pointer to NULL to avoid dangling pointer issues
        }
    }
}
