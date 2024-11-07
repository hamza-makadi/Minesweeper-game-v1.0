#ifndef BUTTONFUNCTIONS_H
#define BUTTONFUNCTIONS_H

#include <SDL.h>
#include "struct.h"

// Function to load and resize an image
SDL_Surface* loadAndResizeImage(const char *file, int width, int height);

/**BUTTON FUNCTIONS**/
// Function to load and resize an image
SDL_Surface* loadAndResizeImage(const char *file, int width, int height);

// Function to create a button
Button createButton(const char *imageFile, const char *text, SDL_Color textColor, int textDirection, float xPercent, float yPercent, int width, int height);

// Function to render a button
void renderButton(SDL_Surface *screen, Button *button);

// Function to free button resources
void freeButton(Button *button);

// Function to handle button clicks
int handleButtonClick(Screen* screen, int mouseX, int mouseY);

/**CHECKBOX FUNCTIONS**/
// Function to create a checkbox
CheckBox createCheckbox(const char *imageFileChecked, const char *imageFileNotChecked, const char *text, SDL_Color textColor, int textDirection, float xPercent, float yPercent, int width, int height);

// Function to render a checkbox
void renderCheckbox(SDL_Surface *screen, CheckBox *checkbox);

// Function to free checkbox resources
void freeCheckbox(CheckBox *checkbox);

// Function to handle checkbox clicks
void handleCheckBoxClick(Screen* screen, int mouseX, int mouseY);

#endif
