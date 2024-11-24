#include "../include/game_manager.h"
#include "../include/struct.h"
#include "../include/button_func.h"
#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL_image.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

/**
 * Initializes the game by setting up the grid, loading the necessary images, and allocating memory for the assets.
 * The function also sets the initial values for the game's properties, such as rows, columns, number of mines, and cell size.
 * It then populates the grid with default cell values and loads the images for the game elements like numbers, bombs, and flags.
 *
 * Parameters:
 *   - Game *game: A pointer to the Game structure that will be initialized.
 */
void initializeGame(Game *game) {
    // Initialize basic game properties
    game->gameState = 0;  // Indicates the game is still on ( the player did not lose yet )
    game->firstClick = 0;  // Indicates if the user has clicked for the first time
    game->rows = gameRowsNum;  // Number of rows in the grid
    game->cols = gameColsNum;  // Number of columns in the grid
    game->numMines = gameMinesNum;  // Number of mines in the grid
    game->flagCount = 0;  // Number of flags
    game->cellSize = cellSize;  // Size of each cell in the grid
    game->startTime = SDL_GetTicks();
    game->elapsedTime = (SDL_GetTicks() - game->startTime) / 1000 ; // elapsed time in seconds
    game->pausedTime = 0;

    // Allocate memory for 12 images representing game assets (numbers, bomb, flag, etc.)
    game->assets = malloc(12 * sizeof(SDL_Surface*));
    if (!game->assets) {
        printf("Memory allocation failed for imageNumbers\n");
        return;  // If memory allocation fails, terminate the function
    }

    // Load images into the asset array (each index corresponds to a specific game asset)
    game->assets[0] = loadAndResizeImage("assets/images/1.jpg", game->cellSize, game->cellSize);
    game->assets[1] = loadAndResizeImage("assets/images/2.jpg", game->cellSize, game->cellSize);
    game->assets[2] = loadAndResizeImage("assets/images/3.jpg", game->cellSize, game->cellSize);
    game->assets[3] = loadAndResizeImage("assets/images/4.jpg", game->cellSize, game->cellSize);
    game->assets[4] = loadAndResizeImage("assets/images/5.jpg", game->cellSize, game->cellSize);
    game->assets[5] = loadAndResizeImage("assets/images/6.jpg", game->cellSize, game->cellSize);
    game->assets[6] = loadAndResizeImage("assets/images/7.jpg", game->cellSize, game->cellSize);
    game->assets[7] = loadAndResizeImage("assets/images/8.jpg", game->cellSize, game->cellSize);
    game->assets[8] = loadAndResizeImage("assets/images/covered.jpg", game->cellSize, game->cellSize);
    game->assets[9] = loadAndResizeImage("assets/images/empty.jpg", game->cellSize, game->cellSize);
    game->assets[10] = loadAndResizeImage("assets/images/bomb.jpg", game->cellSize, game->cellSize);
    game->assets[11] = loadAndResizeImage("assets/images/flag.jpg", game->cellSize, game->cellSize);

    // Allocate memory for the grid (2D array of cells)
    int i, j;
    game->grid = (Cell **)malloc(game->rows * sizeof(Cell *));
    if (!game->grid) {
        printf("Failed to allocate memory\n");
        gameState = GAME_OFF;  // End the game if memory allocation fails
        return;
    }

    // Allocate memory for each row of cells in the grid
    for (i = 0; i < game->rows; i++) {
        game->grid[i] = (Cell *)malloc(game->cols * sizeof(Cell));
        if (!game->grid[i]) {
            printf("Failed to allocate memory\n");
            gameState = GAME_OFF;
            return;
        }

        // Initialize each cell with default values (no mines, not revealed, not flagged, and no adjacent mines)
        for (j = 0; j < game->cols; j++) {
            game->grid[i][j].isMine = 0;         // No mine
            game->grid[i][j].isRevealed = 0;     // Cell not revealed
            game->grid[i][j].isFlagged = 0;      // No flag
            game->grid[i][j].adjacentMines = 0;  // No adjacent mines
        }
    }
}

/**
 * Draws a cell on the game screen based on its current state.
 * The cell can either be revealed, flagged, or covered.
 * The image drawn depends on whether the cell is a mine, has adjacent mines, or is empty.
 *
 * Parameters:
 *   - SDL_Surface *screen: The surface where the cell will be drawn (typically the game window).
 *   - Cell cell: The cell to be drawn, containing its state (revealed, flagged, etc.).
 *   - int x: The x-coordinate where the cell will be drawn on the screen.
 *   - int y: The y-coordinate where the cell will be drawn on the screen.
 *   - Game* game: The game state containing assets and other necessary data.
 */
void drawCell(SDL_Surface *screen, Cell cell, int x, int y, Game* game) {
    // Define the rectangle where the cell will be drawn, using its coordinates and size
    SDL_Rect destRect = {x, y, game->cellSize, game->cellSize};

    // Default image index (no image chosen)
    int ImageIndex = -1;

    // Check the state of the cell and select the appropriate image
    if (cell.isRevealed) {
        if (cell.isMine) {
            // If the cell is revealed and is a mine, use the bomb image
            ImageIndex = 10;
        } else if (cell.adjacentMines > 0) {
            // If the cell is revealed and has adjacent mines, show the number of adjacent mines
            ImageIndex = cell.adjacentMines - 1;
        } else {
            // If the cell is revealed but has no adjacent mines, show the empty cell
            ImageIndex = 9;
        }
    } else if (cell.isFlagged) {
        // If the cell is flagged, show the flag image
        ImageIndex = 11;
    } else {
        // If the cell is not revealed or flagged, show the covered image (cell is hidden)
        ImageIndex = 8;
    }

    // Draw the appropriate image at the given position
    SDL_BlitSurface(game->assets[ImageIndex], NULL, screen, &destRect);
}

void drawTimer(SDL_Surface *screen, Game *game){
    game->elapsedTime =game->pausedTime + (SDL_GetTicks() - game->startTime)/1000 ; // Update elapsed time
    char timeText[10];
    sprintf(timeText, "%02d:%02d", game->elapsedTime / 60, game->elapsedTime % 60);
    SDL_Color textColor = { 250, 250, 250 };
    SDL_Surface* timeSurface = TTF_RenderText_Solid(fonts[3], timeText, textColor);
    SDL_Rect timePosition = { .x = screenWidth-150, .y = 20 };  // Adjust position as needed
    SDL_BlitSurface(timeSurface, NULL, screen, &timePosition);
    SDL_FreeSurface(timeSurface);
}

/**
 * Draws the entire grid of cells on the game screen.
 * It iterates through each cell in the grid and draws it at the appropriate position
 * based on the current row and column, adjusting for cell size.
 *
 * Parameters:
 *   - SDL_Surface *screen: The surface where the grid will be drawn (typically the game window).
 *   - Game *game: The game state containing information about the grid (number of rows, columns, etc.)
 *                 and cell size.
 */
void drawGrid(SDL_Surface *screen, Game *game) {
    // draw the timer on the screen
    drawTimer(screen, game);

    int i, j, shiftX, shiftY;
    // Loop through all rows and columns in the grid
    for (i = 0; i < game->rows; i++) {
        for (j = 0; j < game->cols; j++) {
            shiftX = (screenWidth-game->cellSize*game->cols)/2;
            shiftY = screenHeight-game->cellSize*game->rows-50;
            // Draw each cell at the calculated position (j * cellSize+ shiftX, i * cellSize + shiftY)
            drawCell(screen, game->grid[i][j], (j * game->cellSize)+shiftX , (i * game->cellSize)+shiftY, game);
        }
    }
}

/**
 * Places mines randomly on the grid, ensuring that the first clicked cell does not contain a mine.
 * It generates random coordinates and checks whether the cell already contains a mine or
 * is the first clicked cell before placing a mine.
 *
 * Parameters:
 *   - Game *game: The game state, which includes the grid and the number of mines to place.
 *   - int firstClickRow: The row index of the first clicked cell (to avoid placing a mine there).
 *   - int firstClickCol: The column index of the first clicked cell (to avoid placing a mine there).
 */
void placeMines(Game *game, int firstClickRow, int firstClickCol) {
    srand(time(NULL));  // Seed the random number generator with current time
    int minesPlaced = 0;

    // Place mines until the required number of mines are placed
    while (minesPlaced < game->numMines) {
        // Generate random coordinates for a mine
        int row = rand() % game->rows;
        int col = rand() % game->cols;

        // Ensure the mine is not placed on the first clicked cell
        if ((row == firstClickRow && col == firstClickCol) || game->grid[row][col].isMine) {
            continue; // Skip this iteration if the cell is already a mine or it's the first click
        }

        // Place the mine
        game->grid[row][col].isMine = 1;
        minesPlaced++;  // Increment the mine counter
    }
}

/**
 * Calculates the number of adjacent mines for each cell in the grid.
 * It iterates over all cells, and for each non-mine cell, it checks the 8 neighboring cells
 * (up, down, left, right, and diagonals) to count how many mines are adjacent.
 * The result is stored in each cell's adjacentMines field.
 *
 * Parameters:
 *   - Game *game: The game state, which includes the grid and each cell's properties (mine status, adjacent mines, etc.).
 */
void calculateAdjacentMines(Game *game) {
    int row, col;
    // Iterate over every cell in the grid
    for (row = 0; row < game->rows; row++) {
        for (col = 0; col < game->cols; col++) {
            if (game->grid[row][col].isMine) {
                continue;  // Skip mines, they don't need to count their adjacent mines
            }

            int adjacentMines = 0;

            int i, j;
            // Loop through the 8 possible neighbors (up, down, left, right, and diagonals)
            for (i = -1; i <= 1; i++) {
                for (j = -1; j <= 1; j++) {
                    // Skip the current cell itself
                    if (i == 0 && j == 0) continue;

                    int neighborRow = row + i;
                    int neighborCol = col + j;

                    // Check if the neighbor is within bounds
                    if (neighborRow >= 0 && neighborRow < game->rows && neighborCol >= 0 && neighborCol < game->cols) {
                        // If the neighbor is a mine, increment the adjacent mine count
                        if (game->grid[neighborRow][neighborCol].isMine) {
                            adjacentMines++;
                        }
                    }
                }
            }

            // Store the number of adjacent mines in the current cell
            game->grid[row][col].adjacentMines = adjacentMines;
        }
    }
}

/**
 * Performs a flood fill on the game grid starting from a specified cell.
 * The flood fill reveals all connected cells that have no adjacent mines.
 * It works recursively, revealing adjacent cells when the current cell has no adjacent mines.
 *
 * This implementation is inspired by the flood fill algorithm described in the following Wikipedia page:
 * https://en.wikipedia.org/wiki/Flood_fill#:~:text=The%20traditional%20flood-fill%20algorithm,them%20to%20the%20replacement%20color.
 *
 * Parameters:
 *   - Game *game: The game state, which includes the grid of cells.
 *   - int row: The row index of the starting cell for the flood fill.
 *   - int col: The column index of the starting cell for the flood fill.
 */
void floodFill(Game *game, int row, int col) {
    // If the cell is out of bounds or already revealed, stop the flood fill
    if (row < 0 || row >= game->rows || col < 0 || col >= game->cols) {
        return;
    }

    // If the cell is already revealed, don't reveal it again
    if (game->grid[row][col].isRevealed) {
        return;
    }

    // Reveal the current cell
    game->grid[row][col].isRevealed = 1;

    // If the cell has no adjacent mines, recursively reveal neighboring cells
    if (game->grid[row][col].adjacentMines == 0) {
        // Recursively reveal all adjacent cells
        floodFill(game, row - 1, col);  // Up
        floodFill(game, row + 1, col);  // Down
        floodFill(game, row, col - 1);  // Left
        floodFill(game, row, col + 1);  // Right
        floodFill(game, row - 1, col - 1);  // Top-left diagonal
        floodFill(game, row - 1, col + 1);  // Top-right diagonal
        floodFill(game, row + 1, col - 1);  // Bottom-left diagonal
        floodFill(game, row + 1, col + 1);  // Bottom-right diagonal
    }
}

void saveTimeIfBest(Game* game) {
    FILE* file = fopen("time.dat", "rb"); // Open in binary read mode
    Uint32 times[4]; // To store the current times + new elapsedTime
    int count = 0;

    // Check if file exists and read times
    if (file) {
        while (fread(&times[count], sizeof(Uint32), 1, file) == 1 && count < 3) { // Read each Uint32
            count++;
        }
        fclose(file);
    }

    // Add the new elapsed time
    times[count++] = game->elapsedTime;

    // Sort the times in ascending order
    int i, j;
    for (i = 0; i < count - 1; i++) {
        for (j = i + 1; j < count; j++) {
            if (times[i] > times[j]) {
                Uint32 temp = times[i];
                times[i] = times[j];
                times[j] = temp;
            }
        }
    }

    // Write the best three times back to the file in binary mode
    file = fopen("time.dat", "wb"); // Open in binary write mode
    if (!file) {
        perror("Failed to open file for writing");
        return;
    }
    for (i = 0; i < count && i < 3; i++) {
        fwrite(&times[i], sizeof(Uint32), 1, file); // Write each Uint32 value
    }
    fclose(file);
}

int checkWin(Game* game) {
    int i, j;
    for (i = 0; i < game->rows; i++) {
        for (j = 0; j < game->cols; j++) {
            // If a non-mine cell is not revealed, the player hasn't won
            if ((!game->grid[i][j].isMine) && !(game->grid[i][j].isRevealed)) {
                return 0;
            }
        }
    }
    // save time if it's top 3
    saveTimeIfBest(game);
    return 1; // All conditions satisfied, player has won
}

/**
 * Handles a cell click event in the game. This function determines the clicked cell's position based on the mouse coordinates,
 * checks if the click is within the grid bounds, and processes the click accordingly.
 * If the user clicks for the first time, mines are placed on the grid, ensuring the first clicked cell is not a mine.
 * The flood fill is then performed starting from the clicked cell to reveal all connected empty cells.
 *
 * Parameters:
 *   - Game *game: The current game state, which includes the grid and other necessary data.
 *   - int mouseX: The x-coordinate of the mouse click, which determines the column.
 *   - int mouseY: The y-coordinate of the mouse click, which determines the row.
 */
void handleCellClick(Game *game, int mouseX, int mouseY, PlayerStats *playerStats) {
    // Calculate grid coordinates
    int shiftX = (screenWidth - game->cellSize * game->cols) / 2;
    int shiftY = screenHeight - game->cellSize * game->rows - 50;
    int col = (mouseX - shiftX) / game->cellSize;
    int row = (mouseY - shiftY) / game->cellSize;

    // Check if click is within grid bounds
    if (row < 0 || row >= game->rows || col < 0 || col >= game->cols) {
        return;
    }

    Cell *clickedCell = &game->grid[row][col];

    // First click: Place mines and calculate adjacent mines
    if (!game->firstClick) {
        placeMines(game, row, col);
        calculateAdjacentMines(game);
        game->startTime = SDL_GetTicks();  // Start the timer
        game->firstClick = 1;
    }

    // Process cell based on its state
    if (clickedCell->isFlagged || clickedCell->isRevealed) {
        return;  // Ignore clicks on flagged or already revealed cells
    }

    if (clickedCell->isMine) {
        game->gameState = 1;
        playerStats->gamesPlayed++;
        playerStats->WinStreak = 0;
        currentScreen = 5;
        return;
    }

    if (clickedCell->adjacentMines == 0) {
        floodFill(game, row, col);  // Reveal empty region
    } else {
        clickedCell->isRevealed = 1;  // Reveal the clicked cell
    }
    if (checkWin(game)==1) {
        game->gameState = 2;
        playerStats->gamesPlayed++;
        playerStats->gamesWon++;
        playerStats->WinStreak++;
        currentScreen = 5;
    }
}

void handleFlagClick(Game *game, int mouseX, int mouseY) {
    // Calculate grid coordinates
    int shiftX = (screenWidth - game->cellSize * game->cols) / 2;
    int shiftY = screenHeight - game->cellSize * game->rows - 50;
    int col = (mouseX - shiftX) / game->cellSize;
    int row = (mouseY - shiftY) / game->cellSize;

    // Check if click is within grid bounds
    if (row < 0 || row >= game->rows || col < 0 || col >= game->cols) {
        return;
    }

    Cell *clickedCell = &game->grid[row][col];

    // Process cell based on its state
    if (!clickedCell->isRevealed) {
        clickedCell->isFlagged = !clickedCell->isFlagged;

        // Update the count of flagged cells
        if (clickedCell->isFlagged) {
            game->flagCount++;
        } else {
            game->flagCount--;
        }
    }
}


/**
 * Saves the current game grid, including rows, columns, and cell data, to a binary .dat file.
 * This function also saves the game configuration like the first click state, number of mines, and cell size.
 * The data is written in binary format for efficient storage.
 *
 * Parameters:
 *   - Game *game: The current game state, which includes the grid and other necessary data.
 *   - const char *filename: The path to the file where the game data will be saved.
 */
void saveGameGrid(Game *game, const char *filename) {
    FILE *file = fopen(filename, "wb"); // Open file for binary writing
    if (file == NULL) {
        perror("Error opening file");
        return;
    }

    game->pausedTime = game->elapsedTime;

    // Write rows and cols
    fwrite(&game->gameState, sizeof(int), 1, file);
    fwrite(&game->firstClick, sizeof(int), 1, file);
    fwrite(&game->rows, sizeof(int), 1, file);
    fwrite(&game->cols, sizeof(int), 1, file);
    fwrite(&game->numMines, sizeof(int), 1, file);
    fwrite(&game->flagCount, sizeof(int), 1, file);
    fwrite(&game->cellSize, sizeof(int), 1, file);
    fwrite(&game->pausedTime, sizeof(Uint32), 1, file);

    // Write grid data
    int i,j;
    for (i = 0; i < game->rows; i++) {
        for (j = 0; j < game->cols; j++) {
            // Write each Cell data
            fwrite(&game->grid[i][j], sizeof(Cell), 1, file);
        }
    }

    fclose(file); // Close the file
}

/**
 * Loads the game grid, including rows, columns, and cell data, from a binary .dat file.
 * This function also loads necessary images and reallocates memory for the grid based on the data in the file.
 *
 * Parameters:
 *   - Game *game: The game state object where the loaded data will be stored.
 *   - const char *filename: The path to the file from which the game data will be loaded.
 */
void loadGameGrid(Game *game, const char *filename) {
    FILE *file = fopen(filename, "rb"); // Open file for binary reading
    if (file == NULL) {
        perror("Error opening file");
        return;
    }

    // Read rows and cols
    fread(&game->gameState, sizeof(int), 1, file);
    fread(&game->firstClick, sizeof(int), 1, file);
    fread(&game->rows, sizeof(int), 1, file);
    fread(&game->cols, sizeof(int), 1, file);
    fread(&game->numMines, sizeof(int), 1, file);
    fread(&game->flagCount, sizeof(int), 1, file);
    fread(&game->cellSize, sizeof(int), 1, file);
    fread(&game->pausedTime, sizeof(Uint32), 1, file);


    // Load images
    game->assets[0] = loadAndResizeImage("assets/images/1.jpg", game->cellSize, game->cellSize);
    game->assets[1] = loadAndResizeImage("assets/images/2.jpg", game->cellSize, game->cellSize);
    game->assets[2] = loadAndResizeImage("assets/images/3.jpg", game->cellSize, game->cellSize);
    game->assets[3] = loadAndResizeImage("assets/images/4.jpg", game->cellSize, game->cellSize);
    game->assets[4] = loadAndResizeImage("assets/images/5.jpg", game->cellSize, game->cellSize);
    game->assets[5] = loadAndResizeImage("assets/images/6.jpg", game->cellSize, game->cellSize);
    game->assets[6] = loadAndResizeImage("assets/images/7.jpg", game->cellSize, game->cellSize);
    game->assets[7] = loadAndResizeImage("assets/images/8.jpg", game->cellSize, game->cellSize);
    game->assets[8] = loadAndResizeImage("assets/images/covered.jpg", game->cellSize, game->cellSize);
    game->assets[9] = loadAndResizeImage("assets/images/empty.jpg", game->cellSize, game->cellSize);
    game->assets[10] = loadAndResizeImage("assets/images/bomb.jpg", game->cellSize, game->cellSize);
    game->assets[11] = loadAndResizeImage("assets/images/flag.jpg", game->cellSize, game->cellSize);

    // Allocate memory for the grid
    int i,j;
    game->grid = (Cell **)malloc(game->rows * sizeof(Cell *));
    for (i = 0; i < game->rows; i++) {
        game->grid[i] = (Cell *)malloc(game->cols * sizeof(Cell));
    }

    // Read grid data
    for (i = 0; i < game->rows; i++) {
        for (j = 0; j < game->cols; j++) {
            fread(&game->grid[i][j], sizeof(Cell), 1, file);
        }
    }

    fclose(file); // Close the file
}

/**
 * Frees the allocated memory for the game grid and asset images.
 * This function ensures that all dynamically allocated memory for both the grid and assets is properly released.
 *
 * Parameters:
 *   - Game *game: The game state object containing the allocated memory to be freed.
 */
void freeGameGrid(Game *game) {
    int i;
    // Free memory allocated for images (assets)
    for(i = 0; i < 12; i++) {
        free(game->assets[i]);
    }
    free(game->assets); // Free the array of assets

    // Free memory allocated for the grid cells
    for (i = 0; i < game->rows; i++) {
        free(game->grid[i]);
    }
    free(game->grid); // Free the grid itself
    game->grid = NULL; // Set the grid pointer to NULL to avoid dangling references
}


// Function to save achievements to a file
void saveAchievementsToFile(Achievement achievements[], int totalAchievements, PlayerStats *playerStats) {
    FILE *file = fopen("achievements.dat", "wb"); // Open file in binary write mode
    if (file == NULL) {
        printf("Error: Could not open file achievements.dat for saving achievements.\n");
        return;
    }

    // Write the achievements array to the file
    fwrite(playerStats, sizeof(PlayerStats), 1, file);
    size_t written = fwrite(achievements, sizeof(Achievement), totalAchievements, file);
    if (written != totalAchievements) {
        printf("Error: Could not save all achievements to the file.\n");
    } else {
        printf("Achievements saved successfully to achievements.dat.\n");
    }

    fclose(file); // Close the file
}

void loadAchievementsFromFile(Achievement achievements[], int totalAchievements, PlayerStats *playerStats) {
    FILE *file = fopen("achievements.dat", "rb"); // Open file in binary read mode
    if (file == NULL) {
        printf("No achievement file found. Initializing new achievements.\n");
        initializeAchievements(achievements, playerStats); // Initialize achievements
        return;
    }

    // Read the achievements array from the file
    fread(playerStats, sizeof(PlayerStats), 1, file);
    size_t read = fread(achievements, sizeof(Achievement), totalAchievements, file);
    if (read != totalAchievements) {
        printf("Error: Could not load all achievements from the file.\n");
        initializeAchievements(achievements, playerStats); // Reinitialize if there's an issue
    } else {
        printf("Achievements loaded successfully from achievements.dat.\n");
    }

    fclose(file); // Close the file
}

void initializeAchievements(Achievement achievements[], PlayerStats *playerStats) {
    playerStats->gamesPlayed = 0;
    playerStats->gamesWon = 0;
    playerStats->WinStreak = 0;

    // Achievement 1: Win a game
    strcpy(achievements[0].name, "Win a game");
    achievements[0].isUnlocked = 0;

    // Achievement 2: Win a game under 5 minutes
    strcpy(achievements[1].name, "Win a game under 5 min");
    achievements[1].isUnlocked = 0;

    // Achievement 3: Play 10 games
    strcpy(achievements[2].name, "Play 10 games");
    achievements[2].isUnlocked = 0;

    // Achievement 4: Play 50 games
    strcpy(achievements[3].name, "Play 50 games");
    achievements[3].isUnlocked = 0;

    // Achievement 5: Play 100 games
    strcpy(achievements[4].name, "Play 100 games");
    achievements[4].isUnlocked = 0;

    // Achievement 6: Win 2 games in a row
    strcpy(achievements[5].name, "Win 2 games in a row");
    achievements[5].isUnlocked = 0;

    // Achievement 7: Win 10 games
    strcpy(achievements[6].name, "Win 10 games");
    achievements[6].isUnlocked = 0;

    // Achievement 8: Play 500 games
    strcpy(achievements[7].name, "Play 500 games");
    achievements[7].isUnlocked = 0;

    // Achievement 9: Complete a game under 8 minutes
    strcpy(achievements[8].name, "Win a game under 8 min");
    achievements[8].isUnlocked = 0;

    // Achievement 10: Win 5 games in a row
    strcpy(achievements[9].name, "Win 5 games in a row");
    achievements[9].isUnlocked = 0;

    // Achievement 11: Win 100 games.
    strcpy(achievements[10].name, "Win 100 games");
    achievements[10].isUnlocked = 0;

    // Achievement 12: Win 100 games.
    strcpy(achievements[11].name, "Play 1000 games");
    achievements[11].isUnlocked = 0;

}

void checkAchievements(Achievement achievements[], PlayerStats playerStats, Game game) {
    // Win a game
    if (playerStats.gamesWon == 1 && achievements[0].isUnlocked == 0) {
        achievements[0].isUnlocked = 1;
    }

    // Win a game under 5 minutes
    if (game.gameState == 2 && game.elapsedTime < 300 && achievements[1].isUnlocked == 0) {
        achievements[1].isUnlocked = 1;
    }

    // Play 10 games
    if (playerStats.gamesPlayed == 10 && achievements[2].isUnlocked == 0) {
        achievements[2].isUnlocked = 1;
    }

    // Play 50 games
    if (playerStats.gamesPlayed == 50 && achievements[3].isUnlocked == 0) {
        achievements[3].isUnlocked = 1;
    }

    // Play 100 games
    if (playerStats.gamesPlayed == 100 && achievements[4].isUnlocked == 0) {
        achievements[4].isUnlocked = 1;
    }

    // Win 2 games in a row
    if (playerStats.WinStreak == 2 && achievements[5].isUnlocked == 0) {
        achievements[5].isUnlocked = 1;
    }

    // Win 10 games
    if (playerStats.gamesWon == 10 && achievements[6].isUnlocked == 0) {
        achievements[6].isUnlocked = 1;
    }

     // Play 500 games
    if (playerStats.gamesPlayed == 500 && achievements[7].isUnlocked == 0) {
        achievements[7].isUnlocked = 1;
    }

    // Win a game under 8 minutes
    if (game.gameState == 2 && game.elapsedTime < 480 && achievements[8].isUnlocked == 0) {
        achievements[8].isUnlocked = 1;
    }

    // Win 5 games in a row
    if (playerStats.WinStreak == 5 && achievements[9].isUnlocked == 0) {
        achievements[9].isUnlocked = 1;
    }

    // Win 100 games
    if (playerStats.gamesWon == 100 && achievements[10].isUnlocked == 0) {
        achievements[10].isUnlocked = 1;
    }

    // Play 1000 games
    if (playerStats.gamesPlayed == 1000 && achievements[11].isUnlocked == 0) {
        achievements[11].isUnlocked = 1;
    }
}
