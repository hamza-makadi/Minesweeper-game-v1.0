#include "../include/struct.h"
#include "../include/button_func.h"
#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL_image.h>
#include <string.h>

/**
 * Loads an image from a specified file, resizes it to the given dimensions,
 * and returns the resized image as an SDL_Surface. If the image fails to load,
 * an error message is printed, and the game state is set to GAME_OFF.
 *
 * Parameters:
 *   - const char *file: Path to the image file to load.
 *   - int width: Desired width of the resized image.
 *   - int height: Desired height of the resized image.
 *
 * Returns:
 *   - SDL_Surface*: A pointer to the resized image surface, or NULL if the image failed to load.
 */
SDL_Surface* loadAndResizeImage(const char *file, int width, int height) {

    // Load the image file into a temporary surface
    SDL_Surface *temp = IMG_Load(file);
    if (!temp) {
        // If loading fails, print error message and set game state to GAME_OFF
        printf("Failed to load image: %s\n", IMG_GetError());
        gameState = GAME_OFF;
        return NULL;
    }

    // Create a new surface for the resized image with the specified width and height
    SDL_Surface *resizedImage = SDL_CreateRGBSurface(SDL_SWSURFACE, width, height,
                                                     temp->format->BitsPerPixel,
                                                     temp->format->Rmask,
                                                     temp->format->Gmask,
                                                     temp->format->Bmask,
                                                     temp->format->Amask);

    // Stretch the original image to fit the new surface dimensions
    SDL_SoftStretch(temp, NULL, resizedImage, NULL);

    // Free the temporary surface holding the original image
    SDL_FreeSurface(temp);

    // Return the resized image surface
    return resizedImage;
}


/*******************BUTTON FUNCTIONS*************************/


/**
 * Creates a button with specified image, text, dimensions, color, and position.
 * The button's image is loaded and resized, and its position is calculated based on
 * the provided screen percentages.
 *
 * Parameters:
 *   - const char *imageFile: Path to the image file used as the button's background.
 *   - const char *text: Text displayed on the button.
 *   - SDL_Color textColor: Color of the text displayed on the button.
 *   - int textDirection: Direction in which the text is displayed (left, center, or right).
 *   - float xPercent: X-coordinate percentage position of the button on the screen.
 *   - float yPercent: Y-coordinate percentage position of the button on the screen.
 *   - int width: Width of the button.
 *   - int height: Height of the button.
 *
 * Returns:
 *   - Button: A Button struct instance with initialized properties.
 */
Button createButton(const char *imageFile, const char *text, SDL_Color textColor, int textDirection, float xPercent, float yPercent, int width, int height) {

    // Initialize button properties with provided values
    Button button;

    // Load and resize the button's background image
    button.image = loadAndResizeImage(imageFile, width, height);

    // Set button text and color
    button.text = text;
    button.textColor = textColor;
    button.textDirection = textDirection;

    // Set button state to active
    button.isActive = 1;

    // Set button dimensions
    button.width = width;
    button.height = height;

    // Calculate button position based on screen dimensions and percentage offsets
    button.position.x = (int)((screenWidth - button.width) * xPercent);
    button.position.y = (int)((screenHeight - button.height) * yPercent);

    // Initialize the button's onClick handler to NULL
    button.onClick = NULL;

    // Return the fully initialized button
    return button;
}


/**
 * Renders a button on the screen, including both its background image and text.
 * The function checks if the button has an image and text to render, then
 * draws the image on the screen and positions the text accordingly, based
 * on the specified text alignment direction.
 *
 * Parameters:
 *   - SDL_Surface *screen: The surface where the button will be rendered.
 *   - Button *button: The button to be rendered, which includes its image, text, and position.
 */
void renderButton(SDL_Surface *screen, Button *button) {

    // If the button has an image, render it on the screen at the button's position
    if (button->image) {
        SDL_BlitSurface(button->image, NULL, screen, &button->position);
    }

    // If the button has text, render it as well
    if (strcmp(button->text, "") != 0) {

        // Render the button's text to a surface with the specified font and color
        SDL_Surface *textSurface = TTF_RenderText_Solid(fonts[2], button->text, button->textColor);

        // If text rendering fails, output an error and stop rendering
        if (!textSurface) {
            printf("Unable to render text surface! SDL_ttf Error: %s\n", TTF_GetError());
            gameState = GAME_OFF; // Set game state to off due to error
            return;
        }

        // Define the position for the text
        SDL_Rect textPosition;

        // Align the text based on the specified direction (center or right)
        if (button->textDirection == 0) {
            // Center the text on the button
            textPosition.x = button->position.x + (button->width - textSurface->w) / 2;
            textPosition.y = button->position.y + (button->height - textSurface->h) / 2;
        } else if (button->textDirection == 1) {
            // Position the text to the right of the button
            textPosition.x = button->position.x + button->width + 20;
            textPosition.y = button->position.y + (button->height - textSurface->h) / 2;
        }

        // Blit the rendered text to the screen at the calculated position
        SDL_BlitSurface(textSurface, NULL, screen, &textPosition);

        // Free the text surface after rendering to avoid memory leak
        SDL_FreeSurface(textSurface);
    }
}

/**
 * Frees the resources associated with a button, specifically the image surface.
 * This function helps prevent memory leaks by deallocating the memory used by
 * the button's background image when it is no longer needed.
 *
 * Parameters:
 *   - Button *button: The button whose resources are to be freed.
 */
void freeButton(Button *button) {

    // Free the surface memory allocated for the button's image
    SDL_FreeSurface(button->image);

    // Set the image pointer to NULL to avoid dangling pointer references
    button->image = NULL;
}


/**
 * Handles button click events by checking if the mouse coordinates fall within any button's area.
 * If a button is clicked and has an associated onClick function, that function is called.
 * The function returns 1 if a button was clicked and the onClick function was executed,
 * or 0 if no button was clicked.
 *
 * Parameters:
 *   - Screen *screen: The screen containing the buttons to check for clicks.
 *   - int mouseX: The X-coordinate of the mouse pointer.
 *   - int mouseY: The Y-coordinate of the mouse pointer.
 *
 * Returns:
 *   - int: 1 if a button was clicked and its onClick function was executed, 0 otherwise.
 */
int handleButtonClick(Screen* screen, int mouseX, int mouseY) {
    int i;
    // Loop through each button on the screen
    for (i = 0; i < screen->buttonCount; i++) {
        Button *button = &screen->buttons[i];

        // Check if the mouse is within the bounds of the button
        if (mouseX > button->position.x && mouseX < button->position.x + button->width &&
            mouseY > button->position.y && mouseY < button->position.y + button->height) {

            // If the button has an onClick handler, execute it
            if (button->onClick) {
                button->onClick();
                return 1; // Return 1 to indicate a successful click
            }
        }
    }

    return 0; // Return 0 if no button was clicked
}


/******************CHECKBOX FUNCTIONS*******************/


/**
 * Creates a checkbox with specified checked and unchecked images, text, dimensions, color, and position.
 * The checkbox is initialized with a default unchecked state, and the position is calculated based on
 * the provided screen percentages.
 *
 * Parameters:
 *   - const char *imageFileChecked: Path to the image file used for the checked state of the checkbox.
 *   - const char *imageFileNotChecked: Path to the image file used for the unchecked state of the checkbox.
 *   - const char *text: Text displayed next to the checkbox.
 *   - SDL_Color textColor: Color of the text displayed next to the checkbox.
 *   - int textDirection: Direction in which the text is displayed (left, center, or right).
 *   - float xPercent: X-coordinate percentage position of the checkbox on the screen.
 *   - float yPercent: Y-coordinate percentage position of the checkbox on the screen.
 *   - int width: Width of the checkbox.
 *   - int height: Height of the checkbox.
 *
 * Returns:
 *   - CheckBox: A CheckBox struct instance with initialized properties.
 */
CheckBox createCheckbox(const char *imageFileChecked, const char *imageFileNotChecked, const char *text, SDL_Color textColor, int textDirection, float xPercent, float yPercent, int width, int height) {

    // Initialize checkbox properties with provided values
    CheckBox button;

    // Load and resize the images for the checked and unchecked states of the checkbox
    button.imageChecked = loadAndResizeImage(imageFileChecked, width, height);
    button.imageNotChecked = loadAndResizeImage(imageFileNotChecked, width, height);

    // Set checkbox text, color, and alignment
    button.text = text;
    button.textColor = textColor;
    button.textDirection = textDirection;

    // Set the initial checkbox state to unchecked (0)
    button.isChecked = 0;

    // Set checkbox dimensions
    button.width = width;
    button.height = height;

    // Calculate the checkbox position based on screen dimensions and percentage offsets
    button.position.x = (int)((screenWidth - button.width) * xPercent);
    button.position.y = (int)((screenHeight - button.height) * yPercent);

    // Initialize the checkbox's onClick handler to NULL
    button.onClick = NULL;

    // Return the fully initialized checkbox
    return button;
}


/**
 * Renders a checkbox and its associated text on the screen.
 * The checkbox is drawn based on its checked or unchecked state, and the text is positioned
 * according to the specified direction (left, right, or center).
 *
 * Parameters:
 *   - SDL_Surface *screen: The surface where the checkbox will be rendered.
 *   - CheckBox *button: The checkbox to be rendered, including its images, text, and position.
 */
void renderCheckbox(SDL_Surface *screen, CheckBox *button) {

    // Check if both the checked and unchecked images are loaded
    if (button->imageChecked && button->imageNotChecked) {
        // Render the appropriate image based on the checkbox's state
        if (button->isChecked == 0) {
            // Render the checked state if the checkbox is unchecked
            SDL_BlitSurface(button->imageChecked, NULL, screen, &button->position);
        } else {
            // Render the unchecked state if the checkbox is checked
            SDL_BlitSurface(button->imageNotChecked, NULL, screen, &button->position);
        }
    }

    // If the checkbox has associated text, render it as well
    if (strcmp(button->text, "") != 0) {
        // Create a surface for the checkbox's text with the specified font and color
        SDL_Surface *textSurface = TTF_RenderText_Solid(fonts[2], button->text, button->textColor);

        // If text rendering fails, output an error and stop rendering
        if (!textSurface) {
            printf("Unable to render text surface! SDL_ttf Error: %s\n", TTF_GetError());
            gameState = GAME_OFF; // Set game state to off due to error
            return;
        }

        // Define the position for the text
        SDL_Rect textPosition;

        // Position the text based on the specified direction
        if (button->textDirection == 1) {
            // Position text to the right of the checkbox
            textPosition.x = button->position.x + button->width + 20;
            textPosition.y = button->position.y + (button->height - textSurface->h) / 2;
        } else if (button->textDirection == -1) {
            // Position text to the left of the checkbox
            textPosition.x = button->position.x - textSurface->w - 20;
            textPosition.y = button->position.y + (button->height - textSurface->h) / 2;
        } else {
            // Center the text on the checkbox
            textPosition.x = button->position.x + (button->width - textSurface->w) / 2;
            textPosition.y = button->position.y + (button->height - textSurface->h) / 2;
        }

        // Blit the text surface to the screen
        SDL_BlitSurface(textSurface, NULL, screen, &textPosition);

        // Free the text surface to prevent memory leaks
        SDL_FreeSurface(textSurface);
    }
}


/**
 * Frees the resources associated with a checkbox, including its images for the checked and unchecked states.
 * The images are unloaded from memory to avoid memory leaks.
 *
 * Parameters:
 *   - CheckBox *button: The checkbox whose resources will be freed.
 */
void freeCheckbox(CheckBox *button) {
    // Free the surface for the checked image
    SDL_FreeSurface(button->imageChecked);
    button->imageChecked = NULL; // Set the pointer to NULL after freeing

    // Free the surface for the unchecked image
    SDL_FreeSurface(button->imageNotChecked);
    button->imageNotChecked = NULL; // Set the pointer to NULL after freeing
}


/**
 * Handles checkbox clicks by checking if the mouse click is within the bounds of any checkbox on the screen.
 * If the mouse click is inside a checkbox, the associated onClick function is triggered.
 *
 * Parameters:
 *   - Screen* screen: The screen containing the checkboxes to be checked.
 *   - int mouseX: The X-coordinate of the mouse click.
 *   - int mouseY: The Y-coordinate of the mouse click.
 */
void handleCheckBoxClick(Screen* screen, int mouseX, int mouseY) {
    // Iterate through all the checkboxes on the screen
    int i;
    for (i = 0; i < screen->checkBoxCount; i++) {
        // Get the current checkbox
        CheckBox *checkbox = &screen->checkBoxes[i];

        // Check if the mouse click is within the bounds of the checkbox
        if (mouseX > checkbox->position.x && mouseX < checkbox->position.x + checkbox->width &&
            mouseY > checkbox->position.y && mouseY < checkbox->position.y + checkbox->height) {

            // If the checkbox has an onClick handler, call it
            if (checkbox->onClick) {
                checkbox->onClick(screen);
            }
        }
    }
}




