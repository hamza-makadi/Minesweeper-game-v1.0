#ifndef GAMEMANAGER_H
#define GAMEMANAGER_H

#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL_image.h>
#include "struct.h"

// Function to initialize the game
void initializeGame(Game *game);

// Function to draw the grid
void drawGrid(SDL_Surface *screen, Game *game) ;

// Function to handle each cell click
void handleCellClick(Game *game, int mouseX, int mouseY, PlayerStats *playerStats);

void handleFlagClick(Game *game, int mouseX, int mouseY);

// Function to save rows, cols, and grid data to a .dat file
void saveGameGrid(Game *game, const char *filename);

// Function to load rows, cols, and grid data from a .dat file
void loadGameGrid(Game *game, const char *filename);

// Function to save achievements to a file
void saveAchievementsToFile(Achievement achievements[], int totalAchievements, PlayerStats *playerStats);

// Function to load achievements from file
void loadAchievementsFromFile(Achievement achievements[], int totalAchievements, PlayerStats *playerStats);

void checkAchievements(Achievement achievements[], PlayerStats playerStats, Game game);

// Function to free allocated memory for the grid
void freeGameGrid(Game *game) ;


#endif
