/**
 * The code is still a work in progress, and any additional feedback would greatly help me improve it.
 * Some comments were written with the help of AI, so each function is explained in detail.
 * I used this video to guide me through the development of the game: https://youtu.be/LFU5ZlrR21E
 **/

#include <stdio.h>
#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include "include/struct.h"
#include "include/sdl_init.h"
#include "include/screen_manager.h"
#include "include/button_func.h"
#include "include/background_renderer.h"
#include "include/game_manager.h"

// Global variables definition
int screenWidth = 1100;  // Width of the window
int screenHeight = 700;  // Height of the window

int currentScreen = 0;  // Tracks the current screen (0: menu, 1: game, etc.)
GameState gameState = GAME_ON;  // Initial game state is "on"
int frameTimer = 0;  // Timer for controlling frame rate
GameMode gameMode = MODE_EASY;  // Default game mode is easy

// Game grid and cell size
int gameRowsNum = 0;
int gameColsNum = 0;
int gameMinesNum = 0;
int cellSize = 1;

// Array to store fonts for rendering text
TTF_Font *fonts[NUM_FONTS];

// Main function
int main(int argc, char *argv[]) {
    // Initialize SDL window
    SDL_Surface *window = NULL;
    initialize_sdl(&window);  // Set up SDL window

    // Initialize background images (stars and numbers)
    SDL_Surface *stars = IMG_Load("assets/background/star_field.png");
    SDL_Surface *numbers = IMG_Load("assets/background/numbers.png");

    // Initialize fonts for text rendering
    initializeFonts("assets/fonts/M 8pt.ttf");

    // Load the MP3 file (using Mix_LoadMUS for MP3 files)
    Mix_Music* MainMusic = Mix_LoadMUS("assets/sound/02 BGM #02.mp3");
    Mix_PlayMusic(MainMusic, -1);

    Achievement achievements[TOTAL_ACHIEVEMENTS];
    PlayerStats playerStats;
    int i;

    // Load the achievements
    loadAchievementsFromFile(achievements, TOTAL_ACHIEVEMENTS, &playerStats);

    // Initialize different screens (menu, mode, and game)
    Screen *menuScreen = createMenuScreen();
    Screen *modeScreen = createModeScreen();
    Screen *gameScreen = createGameScreen();
    Screen *gameOverScreen = createGameOverScreen();
    Screen *settingsScreen = createSettingsScreen();
    Screen *achievementScreen = createAchievementScreen(achievements, TOTAL_ACHIEVEMENTS);

    // Initialize the game state
    Game game;
    initializeGame(&game);  // Initialize game logic
    loadGameGrid(&game, "game_data.dat");  // Load previous game data (if any)

    // Main game loop
    while (gameState) {
        frameTimer++;  // Increment frame timer for each loop
        renderBackground(window, stars, numbers);  // Render the background

        // Handle user events (keyboard, mouse, and window resizing)
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {  // If the user closes the window
                gameState = GAME_OFF;  // End the game
            } else if (event.type == SDL_VIDEORESIZE) {  // If the window is resized
                // Handle window resizing and recreate screens based on new size
                resize_window(&window, event);
                menuScreen = createMenuScreen();
                modeScreen = createModeScreen();
                gameScreen = createGameScreen();
                settingsScreen = createSettingsScreen();
                gameOverScreen = createGameOverScreen();
                achievementScreen = createAchievementScreen(achievements, TOTAL_ACHIEVEMENTS);
            } else if (event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT) {  // If left mouse button is clicked
                int mouseX = event.button.x;  // Get mouse X position
                int mouseY = event.button.y;  // Get mouse Y position

                // Handle button clicks based on the current screen
                if(currentScreen == 0) {
                    handleButtonClick(menuScreen, mouseX, mouseY);  // Main menu button click
                    if(currentScreen == 1 && game.gameState != 0){ // if the player already lost the previous game don't open it
                        currentScreen = 0;
                    }
                }
                if(currentScreen == 2) {  // Mode screen button clicks
                    if (handleButtonClick(modeScreen, mouseX, mouseY)) {
                        if(currentScreen==4){
                            initializeGame(&game);  // Initialize the game when play button is clicked
                        }
                    }
                    handleCheckBoxClick(modeScreen, mouseX, mouseY);  // Handle checkbox clicks (game modes)
                }
                if(currentScreen == 3) {
                    handleButtonClick(achievementScreen, mouseX, mouseY);
                }
                if(currentScreen == 4 || currentScreen == 1) {  // Game screen cell clicks
                    handleCellClick(&game, mouseX, mouseY, &playerStats);
                    handleButtonClick(gameScreen, mouseX, mouseY);
                }
                if(currentScreen == 5) {
                    handleButtonClick(gameOverScreen, mouseX, mouseY);  // game over button click
                }
                 if(currentScreen == 6) {
                    handleButtonClick(settingsScreen, mouseX, mouseY);
                    handleCheckBoxClick(settingsScreen, mouseX, mouseY);
                }

            }else if (event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_RIGHT) {  // If right mouse button is clicked
                int mouseX = event.button.x;  // Get mouse X position
                int mouseY = event.button.y;  // Get mouse Y position
                if(currentScreen == 4 || currentScreen == 1) {  // Game screen flags click
                    handleFlagClick(&game, mouseX, mouseY);
                }
            }
        }

        // Render the appropriate screen based on the current screen
        switch(currentScreen) {
            case 0:  // Main menu screen
                renderScreen(menuScreen, window);
                break;
            case 1:  // Game grid screen
                renderScreen(gameScreen, window);
                drawGrid(window, &game);
                break;
            case 2:  // Mode selection screen
                renderScreen(modeScreen, window);
                break;
            case 3:
                achievementScreen = createAchievementScreen(achievements, TOTAL_ACHIEVEMENTS);
                renderScreen(achievementScreen, window);
                break;
            case 4:  // Game screen (after selecting a mode)
                renderScreen(gameScreen, window);
                drawGrid(window, &game);
                break;
            case 5:
                if(game.gameState==1){
                    SDL_Color textColorGrey = { 70, 70, 70 };
                    gameOverScreen->buttons[2] = createButton("assets/images/youlose.png", "", textColorGrey, 0, .5, .2, 300, 169);
                }

                renderScreen(gameOverScreen, window);
                displayBestThreeTimes(window);
                checkAchievements(achievements, playerStats, game) ;

                break;
            case 6: // Settings screen
                renderScreen(settingsScreen, window);
                break;
        }

        // Update the window
        SDL_Flip(window);  // Update the window with rendered content
    }

    // Save the game grid to file when exiting
    saveGameGrid(&game, "game_data.dat");
    //save the achievements
    saveAchievementsToFile(achievements, TOTAL_ACHIEVEMENTS, &playerStats);

    // Free resources (background images, screens, game grid, etc.)
    freeBackground(stars, numbers);
    freeScreen(menuScreen);
    freeScreen(modeScreen);
    freeScreen(gameScreen);
    freeScreen(gameOverScreen);
    freeScreen(achievementScreen);
    freeScreen(settingsScreen);
    freeGameGrid(&game);

    //Free sound
    Mix_FreeMusic(MainMusic);
    Mix_CloseAudio();

    // Cleanup and quit SDL and SDL_image
    cleanupFonts();
    IMG_Quit();
    SDL_Quit();

    return 0;
}
