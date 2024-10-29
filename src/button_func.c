#include "../include/struct.h"
#include "../include/button_func.h"
#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL_image.h>
#include <string.h>

// Function to load and resize an image
SDL_Surface* loadAndResizeImage(const char *file, int width, int height) {
    SDL_Surface *temp = IMG_Load(file);
    if (!temp) {
        printf("Failed to load image: %s\n", IMG_GetError());
        return NULL;
    }

    // Create a new surface for the resized image
    SDL_Surface *resizedImage = SDL_CreateRGBSurface(SDL_SWSURFACE, width, height,
                                                      temp->format->BitsPerPixel,
                                                      temp->format->Rmask,
                                                      temp->format->Gmask,
                                                      temp->format->Bmask,
                                                      temp->format->Amask);

    SDL_SoftStretch(temp, NULL, resizedImage, NULL);
    SDL_FreeSurface(temp);



    return resizedImage;
}

/**BUTTON FUNCTIONS**/
// Function to create a button
Button createButton(const char *imageFile, const char *text, SDL_Color textColor, int textDirection, float xPercent, float yPercent, int width, int height) {
    Button button;
    button.image = loadAndResizeImage(imageFile, width, height);
    button.text = text;
    button.textColor = textColor;
    button.textDirection = textDirection;
    button.isActive = 1;
    button.width = width;
    button.height = height;
    button.position.x = (int)((screenWidth - button.width) * xPercent);
    button.position.y = (int)((screenHeight - button.height) * yPercent);
    button.onClick = NULL;
    return button;
}

// Function to render a button
void renderButton(SDL_Surface *screen, Button *button){
    if (button->image) {
        SDL_BlitSurface(button->image, NULL, screen, &button->position);
    }
    if(strcmp(button->text, "")!=0){

        SDL_Surface *textSurface = TTF_RenderText_Solid(fonts[2], button->text, button->textColor);

        if (!textSurface) {
            printf("Unable to render text surface! SDL_ttf Error: %s\n", TTF_GetError());
            return;
        }
        SDL_Rect textPosition;
        if(button->textDirection == 0){
            // Center the text on the button
            textPosition.x = button->position.x + (button->width - textSurface->w) / 2;
            textPosition.y = button->position.y + (button->height - textSurface->h) / 2;
        }else if(button->textDirection == 1){
            // Text on the right of the button
            textPosition.x = button->position.x + button->width + 20 ;
            textPosition.y = button->position.y + (button->height - textSurface->h) / 2;
        }

        SDL_BlitSurface(textSurface, NULL, screen, &textPosition);

        // Free the text surface
        SDL_FreeSurface(textSurface);
    }
}

// Function to free button resources
void freeButton(Button *button) {
    SDL_FreeSurface(button->image);
    button->image = NULL;
}

// Function to handle button clicks
void handleButtonClick(Screen* screen, int mouseX, int mouseY) {
    int i;
    for (i = 0; i < screen->buttonCount; i++) {
        Button *button = &screen->buttons[i];
        if (mouseX > button->position.x && mouseX < button->position.x + button->width &&
            mouseY > button->position.y && mouseY < button->position.y + button->height) {
            if (button->onClick) {
                button->onClick();
            }
        }
    }
}

/**CHECKBOX FUNCTIONS**/
// Function to create a checkbox
CheckBox createCheckbox(const char *imageFileChecked, const char *imageFileNotChecked, const char *text, SDL_Color textColor, int textDirection, float xPercent, float yPercent, int width, int height) {
    CheckBox button;
    button.imageChecked = loadAndResizeImage(imageFileChecked, width, height);
    button.imageNotChecked = loadAndResizeImage(imageFileNotChecked, width, height);
    button.text = text;
    button.textColor = textColor;
    button.textDirection = textDirection;
    button.isChecked = 0;
    button.width = width;
    button.height = height;
    button.position.x = (int)((screenWidth - button.width) * xPercent);
    button.position.y = (int)((screenHeight - button.height) * yPercent);
    button.onClick = NULL;
    return button;
}

// Function to render a checkbox
void renderCheckbox(SDL_Surface *screen, CheckBox *button){
    if (button->imageChecked && button->imageNotChecked) {
        // image if the checkbox is on or off
        if(button->isChecked == 0){
            SDL_BlitSurface(button->imageChecked, NULL, screen, &button->position);
        }else{
            SDL_BlitSurface(button->imageNotChecked, NULL, screen, &button->position);
        }
    }
    if(strcmp(button->text, "")!=0){
        SDL_Surface *textSurface = TTF_RenderText_Solid(fonts[2], button->text, button->textColor);

        if (!textSurface) {
            printf("Unable to render text surface! SDL_ttf Error: %s\n", TTF_GetError());
            return;
        }
        SDL_Rect textPosition;
        if(button->textDirection == 1){
            // Text on the right of the checkbox
            textPosition.x = button->position.x + button->width + 20 ;
            textPosition.y = button->position.y + (button->height - textSurface->h) / 2;
        }else if(button->textDirection == -1){
            // Text on the left of the checkbox
            textPosition.x = button->position.x + button->width + 20 ;
            textPosition.y = button->position.y + (button->height - textSurface->h) / 2;
        }else{
            //Default state
            // Center the text on the checkbox
            textPosition.x = button->position.x + (button->width - textSurface->w) / 2;
            textPosition.y = button->position.y + (button->height - textSurface->h) / 2;
        }

        SDL_BlitSurface(textSurface, NULL, screen, &textPosition);

        // Free the text surface
        SDL_FreeSurface(textSurface);
    }
}

// Function to free checkbox resources
void freeCheckbox(CheckBox *button) {
    SDL_FreeSurface(button->imageChecked);
    button->imageChecked = NULL;
    SDL_FreeSurface(button->imageNotChecked);
    button->imageNotChecked = NULL;
}

// Function to handle checkbox clicks
void handleCheckBoxClick(Screen* screen, int mouseX, int mouseY) {
    int i;
    for (i = 0; i < screen->checkBoxCount; i++) {
        CheckBox *checkbox = &screen->checkBoxes[i];
        if (mouseX > checkbox->position.x && mouseX < checkbox->position.x + checkbox->width &&
            mouseY > checkbox->position.y && mouseY < checkbox->position.y + checkbox->height) {
            if (checkbox->onClick) {
                checkbox->onClick(screen);
            }
        }
    }
}



