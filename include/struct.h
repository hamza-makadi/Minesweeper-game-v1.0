#ifndef STRUCT_H
#define STRUCT_H

#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL_image.h>
#include <stdlib.h>
#include <stdio.h>

// Global variables
#define MAX_TEXTINPUT_LENGTH 100
#define NUM_FONTS 5

extern int screenWidth;
extern int screenHeight;

extern int GameState; /** 1 : is running / 0 : is off **/
extern int frameTimer; /** Number of frames since the game started **/
extern int currentScreen;/**0 : INIT SCREEN / 1 : MENU SCREEN / 2 : LEVELS SCREEN**/

extern TTF_Font *fonts[NUM_FONTS];

// structure for text input field
typedef struct {
    char text[MAX_TEXTINPUT_LENGTH];
    TTF_Font *font;
    SDL_Color color;
    SDL_Rect position;
    SDL_Surface *screen;
} TextInput;

// Button structure
typedef struct {
    SDL_Surface *image;
    SDL_Rect position;
    int width;
    int height;
    const char *text;
    SDL_Color textColor;
    int textDirection; /**0 : center // 1 : right // -1 : left**/
    int isActive; /**0 : inactive // 1 : active **/
    void (*onClick)();
} Button;

// Button structure
typedef struct {
    SDL_Surface *imageChecked;
    SDL_Surface *imageNotChecked;
    SDL_Rect position;
    int width;
    int height;
    const char *text;
    SDL_Color textColor;
    int textDirection; /**0 : center // 1 : right // -1 : left**/
    int isChecked; /**0 : not checked // 1 : checked **/
    void (*onClick)();
} CheckBox;

//structure to define what elements are in each screen
typedef struct {
    const char *screenName;
    TextInput *textInputs;
    Button *buttons;
    CheckBox *checkBoxes;
    int textInputCount;
    int buttonCount;
    int checkBoxCount;
} Screen;

#endif
