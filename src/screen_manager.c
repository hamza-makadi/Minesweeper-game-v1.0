#include "../include/struct.h"
#include "../include/button_func.h"
#include "../include/screen_manager.h"
#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL_image.h>

void toContinueGameScreen(){
    currentScreen = 1;
}

void toNewGameScreen(){
    currentScreen = 2;
}

void toSettingsScreen(){
    currentScreen = 3;
}

void ExitGame(){
    GameState = 0;
}

Screen* createMenuScreen(){
    // init the screen
    Screen *menuScreen = (Screen*)malloc(sizeof(Screen));
    menuScreen->screenName = "MENU SCREEN";
    menuScreen->textInputCount = 0;
    menuScreen->buttonCount = 4;
    menuScreen->checkBoxCount = 0;

    SDL_Color textColor = { 70, 70, 70};
    // create the necessary elements of the screen
    menuScreen->buttons = malloc(menuScreen->buttonCount * sizeof(Button));
    menuScreen->buttons[0] = createButton("assets/buttons/default-button.png", "Continue", textColor, 0, .5, .3, 300, 75); // Continue button
    menuScreen->buttons[1] = createButton("assets/buttons/default-button.png", "New Game", textColor, 0, .5, .5, 300, 75); // New game button
    menuScreen->buttons[2] = createButton("assets/buttons/default-button.png", "Settings", textColor, 0, .5, .7, 300, 75); // Settings button
    menuScreen->buttons[3] = createButton("assets/buttons/default-button.png", "Exit", textColor, 0, .5, .9, 300, 75); // Exit button

    menuScreen->buttons[0].onClick = toContinueGameScreen;
    menuScreen->buttons[1].onClick = toNewGameScreen;
    menuScreen->buttons[2].onClick = toSettingsScreen;
    menuScreen->buttons[3].onClick = ExitGame;

    return menuScreen;
}

Screen* createModeScreen(){
    // init the screen
    Screen *modeScreen = (Screen*)malloc(sizeof(Screen));
    modeScreen->screenName = "MODE SCREEN";
    modeScreen->textInputCount = 0;
    modeScreen->buttonCount = 2;
    modeScreen->checkBoxCount = 3;

     SDL_Color textColorGrey = { 70, 70, 70};
     SDL_Color textColorWhite = { 250, 250, 250};
     SDL_Color textColorRed = { 250, 0, 0};
    // create the necessary elements of the screen
    modeScreen->buttons = malloc(modeScreen->buttonCount * sizeof(Button));
    modeScreen->buttons[0] = createButton("assets/buttons/default-button.png", "Play", textColorGrey, 0, .9, .9, 300, 75);
    modeScreen->buttons[1] = createButton("assets/buttons/default-button.png", "< Back", textColorRed, 0, .1, .1, 200, 55);

    modeScreen->checkBoxes = malloc(modeScreen->checkBoxCount * sizeof(CheckBox));
    modeScreen->checkBoxes[0] = createCheckbox("assets/buttons/Windows_Toggle_Active.png", "assets/buttons/Windows_Toggle_Selected.png", "Easy Mode (9x9 - 10 mines)", textColorWhite, 1, .3, .3, 40, 40);
    modeScreen->checkBoxes[1] = createCheckbox("assets/buttons/Windows_Toggle_Active.png", "assets/buttons/Windows_Toggle_Selected.png", "Medium Mode (16x16 - 40 mines)", textColorWhite, 1, .3, .5, 40, 40);
    modeScreen->checkBoxes[2] = createCheckbox("assets/buttons/Windows_Toggle_Active.png", "assets/buttons/Windows_Toggle_Selected.png", "Hard Mode (30x16 - 99 mines)", textColorWhite, 1, .3, .7, 40, 40);
    return modeScreen;
}

// Function to render the screen
void renderScreen(Screen *Screen, SDL_Surface *screen) {
    int i;
    for (i = 0; i < Screen->buttonCount; i++) {
        renderButton(screen, &(Screen->buttons[i]));
    }
    for (i = 0; i < Screen->checkBoxCount; i++) {
        renderCheckbox(screen, &(Screen->checkBoxes[i]));
    }
}

// Function to free the screen
void freeScreen(Screen *Screen) {
    int i;
    for (i = 0; i < Screen->buttonCount; i++) {
        freeButton(&(Screen->buttons[i]));
    }
    for (i = 0; i < Screen->checkBoxCount; i++) {
        freeCheckbox(&(Screen->checkBoxes[i]));
    }
}
