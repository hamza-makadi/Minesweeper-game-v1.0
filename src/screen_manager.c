#include "../include/struct.h"
#include "../include/button_func.h"
#include "../include/screen_manager.h"
#include "../include/sdl_init.h"
#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <stdlib.h>


void open_link(const char* url) {
    #ifdef _WIN32
        // Windows
        char command[256];
        sprintf(command, "start %s", url);
        system(command);
    #elif __APPLE__
        // macOS
        char command[256];
        sprintf(command, "open %s", url);
        system(command);
    #else
        // Linux/Unix
        char command[256];
        sprintf(command, "xdg-open %s", url);
        system(command);
    #endif
}

void toMenuGameScreen(){
    currentScreen = 0; // menu screen index
}

void toContinueGameScreen(){
    currentScreen = 1; // game continue screen index
}

void toNewGameScreen(){
    currentScreen = 2; // mode screen index
}

void toAchievementsScreen(){
    currentScreen = 3; // achievements screen index
}

void toSettingsScreen(){
    currentScreen = 6; // Settings screen index
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

void MusicON(Screen *screen){
    Mix_ResumeMusic();
    screen->checkBoxes[1].isChecked = 1;
    screen->checkBoxes[2].isChecked = 0;
}

void MusicOFF(Screen *screen){
    Mix_PauseMusic();
    screen->checkBoxes[1].isChecked = 0;
    screen->checkBoxes[2].isChecked = 1;
}

void  toGameRepo(){
    open_link("https://github.com/hamza-makadi/Minesweeper-game-v1.0");
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
    menuScreen->buttonCount = 6;  // 5 buttons for the menu options and 1 for the logo (it's not an actual button just to display the logo)
    menuScreen->checkBoxCount = 0;  // No checkboxes on the menu screen

    SDL_Color textColor = { 70, 70, 70 };  // Set the color for button text

    // Allocate memory for the buttons and create them with respective labels and positions
    menuScreen->buttons = malloc(menuScreen->buttonCount * sizeof(Button));
    menuScreen->buttons[4] = createButton("assets/logo.png", "", textColor, 0, .5, .1, 700, 81); // Exit button
    menuScreen->buttons[0] = createButton("assets/buttons/default-button.png", "Continue", textColor, 0, .5, .3, 300, 75); // Continue button
    menuScreen->buttons[1] = createButton("assets/buttons/default-button.png", "New Game", textColor, 0, .5, .45, 300, 75); // New game button
    menuScreen->buttons[2] = createButton("assets/buttons/default-button.png", "Achievements", textColor, 0, .5, .6, 300, 75); // Achievements button
    menuScreen->buttons[5] = createButton("assets/buttons/default-button.png", "Settings", textColor, 0, .5, .75, 300, 75); // Settings button
    menuScreen->buttons[3] = createButton("assets/buttons/default-button.png", "Exit", textColor, 0, .5, .9, 300, 75); // Exit button

    // Assign callback functions for each button's onClick event
    menuScreen->buttons[0].onClick = toContinueGameScreen;
    menuScreen->buttons[1].onClick = toNewGameScreen;
    menuScreen->buttons[2].onClick = toAchievementsScreen;
    menuScreen->buttons[5].onClick = toSettingsScreen;
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
    gameScreen->buttonCount =2;
    gameScreen->checkBoxCount =0;

     // Define colors for text elements
    SDL_Color textColorGrey = { 70, 70, 70 };

    // Allocate memory for the buttons and create them with respective labels and positions
    gameScreen->buttons = malloc(gameScreen->buttonCount * sizeof(Button));
    gameScreen->buttons[0] = createButton("assets/Window.png", "", textColorGrey, 0, .5, .5, screenWidth, screenHeight);
    gameScreen->buttons[1] = createButton("assets/buttons/small_button.png", " | | ", textColorGrey, 0, .02, .02, 75, 75);

    gameScreen->buttons[1].onClick = toMenuGameScreen ;

    return gameScreen;  // Return the initialized game screen
}

/**
 * Creates and initializes the game over screen for the Minesweeper game.
 * This function sets up the basic structure of the game over screen with a name
 *
 * Returns:
 *   - Screen*: A pointer to the created game screen object.
 */
Screen* createGameOverScreen() {
    // Initialize the screen structure for the game screen
    Screen *gameOverScreen = (Screen*)malloc(sizeof(Screen));
    gameOverScreen->screenName = "GAME OVER SCREEN";  // Set the screen name
    gameOverScreen->buttonCount =3;
    gameOverScreen->checkBoxCount =0;

     // Define colors for text elements
    SDL_Color textColorGrey = { 70, 70, 70 };

     // Allocate memory for the buttons and create them with respective labels and positions
    gameOverScreen->buttons = malloc(gameOverScreen->buttonCount * sizeof(Button));
    gameOverScreen->buttons[0] = createButton("assets/Window.png", "", textColorGrey, 0, .5, .5, screenWidth - (screenWidth * 0.5), screenHeight - (screenHeight * 0.2));
    gameOverScreen->buttons[1] = createButton("assets/buttons/default-button.png", "Play Again", textColorGrey, 0, .5, .8, 300, 75);
    gameOverScreen->buttons[2] = createButton("assets/images/youwin.png", "", textColorGrey, 0, .5, .2, 300, 169);

    gameOverScreen->buttons[1].onClick = toNewGameScreen;

    return gameOverScreen;  // Return the initialized game screen
}

Screen* createAchievementScreen(Achievement achievements[], int totalAchievements){
    // Initialize the screen structure for the game screen
    Screen *achievementsScreen = (Screen*)malloc(sizeof(Screen));

    achievementsScreen->screenName = "ACHIEVEMENTS SCREEN";  // Set the screen name
    achievementsScreen->buttonCount = 3;
    achievementsScreen->checkBoxCount = totalAchievements;

    // Define colors for text elements
    SDL_Color textColorWhite = { 250, 250, 250 };

    // Define buttons
    achievementsScreen->buttons = malloc(achievementsScreen->buttonCount * sizeof(Button));
    achievementsScreen->buttons[0] = createButton("assets/Window.png", "", textColorWhite, 0, .5, .5, screenWidth - (screenWidth * 0.1), screenHeight - (screenHeight * 0.1));  // Window button
    achievementsScreen->buttons[1] = createButton("assets/buttons/small_button.png", "", textColorWhite, 0, .1, .1, 50, 50);  // Small button
    achievementsScreen->buttons[2] = createButton("assets/buttons/close_button.png", "", textColorWhite, 0, .1, .1, 45, 45);  // Close button

    achievementsScreen->checkBoxes = malloc(achievementsScreen->checkBoxCount * sizeof(CheckBox));
    int i;
    float offsetX, offsetY;
    for( i=0; i<totalAchievements; i++){
        offsetX = (i < 6) ? .1 : .6;
        offsetY = (i >= 6) ? .3+((i%6)/10.0) : .3+(i/10.0);
        achievementsScreen->checkBoxes[i] = createCheckbox("assets/buttons/Windows_Toggle_Active.png", "assets/buttons/Windows_Toggle_Selected.png", achievements[i].name, textColorWhite, 1, offsetX, offsetY, 40, 40);
        achievementsScreen->checkBoxes[i].isChecked = achievements[i].isUnlocked;
    }

    achievementsScreen->buttons[2].onClick = toMenuGameScreen;

    return achievementsScreen;
}

Screen* createSettingsScreen(Achievement achievements[], int totalAchievements){
    // Initialize the screen structure for the game screen
    Screen *SettingsScreen = (Screen*)malloc(sizeof(Screen));

    SettingsScreen->screenName = "SETTINGS SCREEN";  // Set the screen name
    SettingsScreen->buttonCount = 4;
    SettingsScreen->checkBoxCount = 3;

    // Define colors for text elements
    SDL_Color textColorWhite = { 250, 250, 250 };
    SDL_Color textColorGrey = { 70, 70, 70 };

    // Define buttons
    SettingsScreen->buttons = malloc(SettingsScreen->buttonCount * sizeof(Button));
    SettingsScreen->buttons[0] = createButton("assets/Window.png", "", textColorWhite, 0, .5, .5, screenWidth - (screenWidth * 0.1), screenHeight - (screenHeight * 0.1));  // Window button
    SettingsScreen->buttons[1] = createButton("assets/buttons/small_button.png", "", textColorWhite, 0, .1, .1, 50, 50);  // Small button
    SettingsScreen->buttons[2] = createButton("assets/buttons/close_button.png", "", textColorWhite, 0, .1, .1, 45, 45);  // Close button
    SettingsScreen->buttons[3] = createButton("assets/buttons/default-button.png", "Open Game Repository", textColorGrey, 0, .5, .9, 300, 75);

    SettingsScreen->buttons[2].onClick = toMenuGameScreen;
    SettingsScreen->buttons[3].onClick = toGameRepo;

    // Allocate memory for the checkboxes and create them with respective labels and positions
    SettingsScreen->checkBoxes = malloc(SettingsScreen->checkBoxCount * sizeof(CheckBox));
    SettingsScreen->checkBoxes[0] = createCheckbox("", "", "Music : ", textColorWhite, 1, .2, .5, 40, 40);  //Music Text

    SettingsScreen->checkBoxes[1] = createCheckbox("assets/buttons/Windows_Toggle_Active.png", "assets/buttons/Windows_Toggle_Selected.png", "ON", textColorWhite, 1, .4, .5, 40, 40);  // Music on checkbox
    SettingsScreen->checkBoxes[1].isChecked = 1;
    SettingsScreen->checkBoxes[2] = createCheckbox("assets/buttons/Windows_Toggle_Active.png", "assets/buttons/Windows_Toggle_Selected.png", "OFF", textColorWhite, 1, .6, .5, 40, 40);  // Music off checkbox

    SettingsScreen->checkBoxes[1].onClick = MusicON;
    SettingsScreen->checkBoxes[2].onClick = MusicOFF;
    return SettingsScreen;
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

void displayBestThreeTimes(SDL_Surface *screen) {
    FILE *file = fopen("time.dat", "rb");
    if (!file) {
        fprintf(stderr, "Error: Could not open time.dat\n");
        gameState = GAME_OFF;
        return;
    }

    // Read three Uint32 values from the binary file time.dat
    Uint32 values[3];
    if (fread(values, sizeof(Uint32), 3, file) != 3) {
        fprintf(stderr, "Error: Could not read three values from time.dat\n");
        fclose(file);
        return;
    }
    fclose(file);

    // Convert the values to strings (minutes:seconds format)
    char text1[20], text2[20], text3[20];
    sprintf(text1, "%02u:%02u", values[0] / 60, values[0] % 60); // Use %u for Uint32
    sprintf(text2, "%02u:%02u", values[1] / 60, values[1] % 60);
    sprintf(text3, "%02u:%02u", values[2] / 60, values[2] % 60);

    SDL_Color color = {255, 255, 255}; // White color for text
    // Render the text to SDL_Surfaces
    SDL_Surface *textSurface1 = TTF_RenderText_Solid(fonts[3], text1, color);
    SDL_Surface *textSurface2 = TTF_RenderText_Solid(fonts[3], text2, color);
    SDL_Surface *textSurface3 = TTF_RenderText_Solid(fonts[3], text3, color);

    if (!textSurface1 || !textSurface2 || !textSurface3) {
        fprintf(stderr, "Error: Could not render text surfaces\n");
        gameState = GAME_OFF;
        return;
    }

    // Calculate positions to center the text
    int centerX = screen->w / 2;
    int centerY = screen->h / 2;

    SDL_Rect pos1 = {centerX - textSurface1->w / 2, centerY - textSurface1->h - 10, 0, 0};
    SDL_Rect pos2 = {centerX - textSurface2->w / 2, centerY, 0, 0};
    SDL_Rect pos3 = {centerX - textSurface3->w / 2, centerY + textSurface2->h + 10, 0, 0};

    // Blit the text to the screen
    SDL_BlitSurface(textSurface1, NULL, screen, &pos1);
    SDL_BlitSurface(textSurface2, NULL, screen, &pos2);
    SDL_BlitSurface(textSurface3, NULL, screen, &pos3);

    // Free the surfaces
    SDL_FreeSurface(textSurface1);
    SDL_FreeSurface(textSurface2);
    SDL_FreeSurface(textSurface3);
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

    // Free the screen object itself
    free(Screen);
}
