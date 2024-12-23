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
#define TOTAL_ACHIEVEMENTS 12

extern int screenWidth;
extern int screenHeight;

typedef enum {
    GAME_OFF = 0,
    GAME_ON = 1
} GameState;

typedef enum {
    MODE_EASY = 0,
    MODE_MEDIUM = 1,
    MODE_HARD = 2
} GameMode;

extern GameState gameState;
extern int frameTimer; /** Number of frames since the game started **/
extern int currentScreen;/**0 : INIT SCREEN / 1 : MENU SCREEN / 2 : LEVELS SCREEN**/

extern GameMode gameMode;

extern int gameRowsNum;                       // Number of rows in the grid
extern int gameColsNum;                       // Number of columns in the grid
extern int gameMinesNum;                   // Number of mines
extern int cellSize;                                // cell size based on the dimension of the grid to fit the screen

extern TTF_Font *fonts[NUM_FONTS];

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

// Checkbox structure
typedef struct {
    SDL_Surface *imageChecked;
    SDL_Surface *imageNotChecked;
    SDL_Rect position;
    int width;
    int height;
    const char *text;
    SDL_Color textColor;
    int textDirection; /** 0 : center // 1 : right // -1 : left**/
    int isChecked; /** 0 : not checked // 1 : checked **/
    void (*onClick)();
} CheckBox;

//structure to define what elements are in each screen
typedef struct {
    const char *screenName;
    Button *buttons;
    CheckBox *checkBoxes;
    int buttonCount;
    int checkBoxCount;
} Screen;

// Structure for one cell
typedef struct {
    int isMine;           // 1 if the cell contains a mine, 0 otherwise
    int isRevealed;       // 1 if the cell has been revealed, 0 otherwise
    int isFlagged;        // 1 if the cell has been flagged as a potential mine
    int adjacentMines;    // Number of mines in adjacent cells
} Cell;

// Struct to hold game data
typedef struct {
    int gameState;               // if the game in a win or lose state or still on (stillOn : 0 // lose : 1 // win : 2)
    int firstClick;                  // First click for the user
    int rows;                       // Number of rows in the grid
    int cols;                       // Number of columns in the grid
    int numMines;             // Number of mines based on game mode
    int flagCount;             // Number of flags
    int cellSize;                // Size of the cell
    Uint32 startTime;       // time when the game start
    Uint32 elapsedTime;  // time since the game started
    Uint32 pausedTime;  // time when the player pauses the game
    SDL_Surface **assets; // Images of the game like bomb,numbers and empty cell
    Cell **grid;                 // Pointer to a 2D array of Cells
} Game;
// struct for achievement and player stats
typedef struct {
    char name[50];
    int isUnlocked;  // 0 = locked, 1 = unlocked
} Achievement;

typedef struct {
    int gamesPlayed;
    int gamesWon;
    int WinStreak;
} PlayerStats;

#endif
