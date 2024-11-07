#include "../include/struct.h"
#include "../include/button_func.h"
#include "../include/screen_manager.h"
#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL_image.h>
void toMenuGameScreen(){
    currentScreen = 0; // menu screen index
}

void toContinueGameScreen(){
    currentScreen = 1; // game continue screen index
}

void toNewGameScreen(){
    currentScreen = 2; // mode screen index
}

void toSettingsScreen(){
    currentScreen = 3; // settings screen index
}

void ExitGame(){
    gameState = GAME_OFF;
}

void ActiveEasyMode(Screen *screen){
    screen->checkBoxes[0].isChecked = !screen->checkBoxes[0].isChecked;
    screen->checkBoxes[1].isChecked = 0;
    screen->checkBoxes[2].isChecked = 0;
    gameMode = MODE_EASY;
}

void ActiveMediumMode(Screen *screen){
    screen->checkBoxes[0].isChecked = 0;
    screen->checkBoxes[1].isChecked = !screen->checkBoxes[1].isChecked;
    screen->checkBoxes[2].isChecked = 0;
    gameMode = MODE_MEDIUM;
}

void ActiveHardMode(Screen *screen){
    screen->checkBoxes[0].isChecked = 0;
    screen->checkBoxes[1].isChecked = 0;
    screen->checkBoxes[2].isChecked = !screen->checkBoxes[2].isChecked;
    gameMode = MODE_HARD;
}

void openGame(){
    switch(gameMode){
    case MODE_EASY:
        gameRowsNum = 9;
        gameColsNum = 9;
        gameMinesNum = 10;
        cellSize = 50;
        break;
    case MODE_MEDIUM:
        gameRowsNum = 16;
        gameColsNum = 16;
        gameMinesNum = 40;
        cellSize = 40;
        break;
    case MODE_HARD:
        gameRowsNum = 16;
        gameColsNum = 30;
        gameMinesNum = 99;
        cellSize = 30;
        break;
    }
    currentScreen = 4;
}

/**
 * Creates and initializes the menu screen for the game.
 * This function sets up the menu screen with buttons for continuing, starting a new game, accessing settings, and exiting the game.
 * It also assigns callback functions to each button's onClick event.
 *
 * Returns:
 *   - Screen*: A pointer to the created menu screen object.
 */
Screen* createMenuScreen() {
    // Initialize the screen structure for the menu
    Screen *menuScreen = (Screen*)malloc(sizeof(Screen));
    menuScreen->screenName = "MENU SCREEN";  // Set the screen name
    menuScreen->textInputCount = 0;  // No text inputs on the menu screen
    menuScreen->buttonCount = 4;  // 4 buttons for the menu options
    menuScreen->checkBoxCount = 0;  // No checkboxes on the menu screen

    SDL_Color textColor = { 70, 70, 70 };  // Set the color for button text

    // Allocate memory for the buttons and create them with respective labels and positions
    menuScreen->buttons = malloc(menuScreen->buttonCount * sizeof(Button));
    menuScreen->buttons[0] = createButton("assets/buttons/default-button.png", "Continue", textColor, 0, .5, .3, 300, 75); // Continue button
    menuScreen->buttons[1] = createButton("assets/buttons/default-button.png", "New Game", textColor, 0, .5, .5, 300, 75); // New game button
    menuScreen->buttons[2] = createButton("assets/buttons/default-button.png", "Settings", textColor, 0, .5, .7, 300, 75); // Settings button
    menuScreen->buttons[3] = createButton("assets/buttons/default-button.png", "Exit", textColor, 0, .5, .9, 300, 75); // Exit button

    // Assign callback functions for each button's onClick event
    menuScreen->buttons[0].onClick = toContinueGameScreen;
    menuScreen->buttons[1].onClick = toNewGameScreen;
    menuScreen->buttons[2].onClick = toSettingsScreen;
    menuScreen->buttons[3].onClick = ExitGame;

    return menuScreen;  // Return the initialized menu screen
}
/**
 * Creates and initializes the mode screen for the game.
 * This function sets up the screen with buttons for playing, closing the game, and navigating back to the menu.
 * It also creates checkboxes for selecting different game modes: Easy, Medium, and Hard.
 * Each checkbox has an associated onClick event to toggle the respective mode.
 *
 * Returns:
 *   - Screen*: A pointer to the created mode screen object.
 */
Screen* createModeScreen() {
    // Initialize the screen structure for the mode selection screen
    Screen *modeScreen = (Screen*)malloc(sizeof(Screen));
    modeScreen->screenName = "MODE SCREEN";  // Set the screen name
    modeScreen->textInputCount = 0;  // No text inputs on this screen
    modeScreen->buttonCount = 4;  // 4 buttons for the mode screen options
    modeScreen->checkBoxCount = 3;  // 3 checkboxes for the different difficulty modes

    // Define colors for text elements
    SDL_Color textColorGrey = { 70, 70, 70 };
    SDL_Color textColorWhite = { 250, 250, 250 };
    SDL_Color textColorRed = { 250, 0, 0 };

    // Allocate memory for the buttons and create them with respective labels and positions
    modeScreen->buttons = malloc(modeScreen->buttonCount * sizeof(Button));
    modeScreen->buttons[0] = createButton("assets/Window.png", "", textColorGrey, 0, .5, .5, screenWidth - (screenWidth * 0.1), screenHeight - (screenHeight * 0.1));  // Window button
    modeScreen->buttons[1] = createButton("assets/buttons/default-button.png", "Play", textColorGrey, 0, .9, .9, 300, 75);  // Play button
    modeScreen->buttons[2] = createButton("assets/buttons/small_button.png", "", textColorRed, 0, .1, .1, 50, 50);  // Small button
    modeScreen->buttons[3] = createButton("assets/buttons/close_button.png", "", textColorRed, 0, .1, .1, 45, 45);  // Close button

    // Assign the onClick event for the "to Menu" button
    modeScreen->buttons[2].onClick = toMenuGameScreen;

    // Allocate memory for the checkboxes and create them with respective labels and positions
    modeScreen->checkBoxes = malloc(modeScreen->checkBoxCount * sizeof(CheckBox));
    modeScreen->checkBoxes[0] = createCheckbox("assets/buttons/Windows_Toggle_Active.png", "assets/buttons/Windows_Toggle_Selected.png", "Easy Mode (9x9 - 10 mines)", textColorWhite, 1, .2, .3, 40, 40);  // Easy Mode checkbox
    modeScreen->checkBoxes[1] = createCheckbox("assets/buttons/Windows_Toggle_Active.png", "assets/buttons/Windows_Toggle_Selected.png", "Medium Mode (16x16 - 40 mines)", textColorWhite, 1, .2, .5, 40, 40);  // Medium Mode checkbox
    modeScreen->checkBoxes[2] = createCheckbox("assets/buttons/Windows_Toggle_Active.png", "assets/buttons/Windows_Toggle_Selected.png", "Hard Mode (30x16 - 99 mines)", textColorWhite, 1, .2, .7, 40, 40);  // Hard Mode checkbox

    // Set the default checkbox for Easy Mode to be checked
    modeScreen->checkBoxes[0].isChecked = 1;

    // Assign the onClick event for each checkbox
    modeScreen->checkBoxes[0].onClick = ActiveEasyMode;
    modeScreen->checkBoxes[1].onClick = ActiveMediumMode;
    modeScreen->checkBoxes[2].onClick = ActiveHardMode;

    // Assign the onClick event for the Play button
    modeScreen->buttons[1].onClick = openGame;

    return modeScreen;  // Return the initialized mode screen
}

/**
 * Creates and initializes the game screen for the Minesweeper game.
 * This function sets up the basic structure of the game screen with a name
 *
 * Returns:
 *   - Screen*: A pointer to the created game screen object.
 */
Screen* createGameScreen() {
    // Initialize the screen structure for the game screen
    Screen *gameScreen = (Screen*)malloc(sizeof(Screen));
    gameScreen->screenName = "GAME SCREEN";  // Set the screen name

    // The game screen is empty here, more elements (like buttons, grid, etc.) can be added in the future.

    return gameScreen;  // Return the initialized game screen
}

/**
 * Renders the elements (buttons, checkboxes) on the screen.
 * This function iterates through the buttons and checkboxes in the given screen and renders them using the renderButton
 * and renderCheckbox functions.
 *
 * Parameters:
 *   - Screen: A pointer to the screen object that contains the elements (buttons, checkboxes) to be rendered.
 *   - screen: A pointer to the SDL_Surface where the screen will be rendered (this is typically the window surface).
 */
void renderScreen(Screen *Screen, SDL_Surface *screen) {
    int i;

    // Render all the buttons in the screen
    for (i = 0; i < Screen->buttonCount; i++) {
        renderButton(screen, &(Screen->buttons[i]));  // Call renderButton for each button
    }

    // Render all the checkboxes in the screen
    for (i = 0; i < Screen->checkBoxCount; i++) {
        renderCheckbox(screen, &(Screen->checkBoxes[i]));  // Call renderCheckbox for each checkbox
    }
}

/**
 * Frees the dynamically allocated memory for the screen and its elements.
 * This function iterates through the buttons and checkboxes in the given screen and frees their memory using
 * freeButton and freeCheckbox functions. It then frees the memory for the screen itself.
 *
 * Parameters:
 *   - Screen: A pointer to the screen object whose elements and memory will be freed.
 */
void freeScreen(Screen *Screen) {
    int i;

    // Free memory for all buttons in the screen
    for (i = 0; i < Screen->buttonCount; i++) {
        freeButton(&(Screen->buttons[i]));  // Free each button
    }

    // Free memory for all checkboxes in the screen
    for (i = 0; i < Screen->checkBoxCount; i++) {
        freeCheckbox(&(Screen->checkBoxes[i]));  // Free each checkbox
    }

    // Free other dynamically allocated memory for text inputs (if any)
    free(Screen->buttons);        // Free the buttons array
    free(Screen->checkBoxes);     // Free the checkboxes array
    free(Screen->textInputs);     // Free the text inputs array (if used)

    // Free the screen object itself
    free(Screen);
}
