#include <stdio.h>
#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL_image.h>
#include "include/struct.h"
#include "include/sdl_init.h"
#include "include/screen_manager.h"
#include "include/background_renderer.h"

// Global variables definition
int screenWidth = 800;
int screenHeight = 600;

int currentScreen = 0;
int GameState = 1;
int frameTimer = 0;

TTF_Font *fonts[NUM_FONTS];

int main(int argc, char *argv[]) {
    // Initialize SDL window
    SDL_Surface *window = NULL;
    initialize_sdl(&window);

    //initialize background images
    SDL_Surface *stars = IMG_Load("assets/background/star_field.png");
    SDL_Surface *numbers = IMG_Load("assets/background/numbers.png");

    // initialize font
    initializeFonts("assets/fonts/M 8pt.ttf");

    //initialize screens
    Screen *menuScreen = createMenuScreen();
    Screen *modeScreen = createModeScreen();

    while (GameState) {

        frameTimer++;
        renderBackground(window, stars, numbers);

        // handle user events ( keyboard and mouse events )
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                GameState = 0;
            }else if (event.type == SDL_VIDEORESIZE) {
                // Handle window resizing
                resize_window(&window, event);
                // recreating screens to position the elements relative to the new screen size
                menuScreen = createMenuScreen();
                modeScreen = createModeScreen();
            }else if (event.type == SDL_MOUSEBUTTONDOWN) {
                int mouseX = event.button.x;
                int mouseY = event.button.y;
                // handle buttons clicks only for the current screen
                if(currentScreen==0) {
                        handleButtonClick(menuScreen, mouseX, mouseY);
                }
                if(currentScreen==2){
                    handleButtonClick(modeScreen, mouseX, mouseY);
                    handleCheckBoxClick(modeScreen, mouseX, mouseY);
                }
            }
        }
        switch(currentScreen){
        case 0:
            renderScreen(menuScreen, window);
            break;
        case 2:
            renderScreen(modeScreen, window);
            break;
        }

        // Update the window
        SDL_Flip(window);
    }
    freeBackground(stars, numbers);
    freeScreen(menuScreen);
    freeScreen(modeScreen);
    cleanupFonts();
    SDL_Quit();
    return 0;
}
