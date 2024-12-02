#include "Button.h"
#include <iostream>

// Constructor
Button::Button(const std::string& text, int xPos, int yPos, int w, int h, SDL_Color buttonColor, SDL_Color txtColor, SDL_Color hoverCol, SDL_Color borderCol, int borderWidth, TTF_Font* btnFont)
    : label(text), x(xPos), y(yPos), width(w), height(h), color(buttonColor), textColor(txtColor), hoverColor(hoverCol), borderColor(borderCol), borderThickness(borderWidth), font(btnFont), isHovered(false), onClick(nullptr) {}

// Set callback for button click
void Button::setOnClick(std::function<void()> callback) {
    onClick = callback;
}

// Handle SDL events (like mouse clicks)
void Button::handleEvent(const SDL_Event& e) {
    int mouseX = e.motion.x;
    int mouseY = e.motion.y;
    isHovered = (mouseX >= x && mouseX <= x + width && mouseY >= y && mouseY <= y + height);

    // Check if the event is a mouse button down event
    if (e.type == SDL_MOUSEBUTTONDOWN && isHovered) {
        if (onClick) {
            onClick();
        } else {
            std::cout << "No action assigned to the button \"" << label << "\".\n";
        }
    }
}

// Render the button using SDL
void Button::render(SDL_Renderer* renderer) const {
    // Change button color if mouse is hovering
    SDL_Color currentColor = isHovered ? hoverColor : color;
    
    // Draw the button rectangle
    SDL_Rect buttonRect = { x, y, width, height };
    SDL_SetRenderDrawColor(renderer, currentColor.r, currentColor.g, currentColor.b, currentColor.a);
    SDL_RenderFillRect(renderer, &buttonRect);

    // Render the border
    if (borderThickness > 0) {
        SDL_SetRenderDrawColor(renderer, borderColor.r, borderColor.g, borderColor.b, borderColor.a);
        SDL_Rect borderRect = { x - borderThickness, y - borderThickness, width + 2 * borderThickness, height + 2 * borderThickness };
        SDL_RenderDrawRect(renderer, &borderRect);
    }

    // Render the text
    SDL_Surface* textSurface = TTF_RenderText_Solid(font, label.c_str(), textColor);
    SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
    SDL_Rect textRect = { x + (width - textSurface->w) / 2, y + (height - textSurface->h) / 2, textSurface->w, textSurface->h };
    SDL_RenderCopy(renderer, textTexture, NULL, &textRect);

    // Free resources
    SDL_FreeSurface(textSurface);
    SDL_DestroyTexture(textTexture);
}

// Getters for button properties
int Button::getX() const { return x; }
int Button::getY() const { return y; }
int Button::getWidth() const { return width; }
int Button::getHeight() const { return height; }
bool Button::getIsHovered() const { return isHovered; }
std::string Button::getLabel() const { return label; }
