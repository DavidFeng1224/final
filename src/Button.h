#ifndef BUTTON_H
#define BUTTON_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <functional>
#include <string>

class Button {
private:
    std::string label;
    int x, y, width, height;
    SDL_Color color;
    SDL_Color textColor;
    SDL_Color hoverColor;
    SDL_Color borderColor;
    int borderThickness;
    bool isHovered;
    TTF_Font* font;
    std::function<void()> onClick;

public:
    Button(const std::string& text, int xPos, int yPos, int w, int h, SDL_Color buttonColor, SDL_Color txtColor, SDL_Color hoverCol, SDL_Color borderCol, int borderWidth, TTF_Font* btnFont);

    void setOnClick(std::function<void()> callback);
    void handleEvent(const SDL_Event& e);
    void render(SDL_Renderer* renderer) const;
    
    int getX() const;
    int getY() const;
    int getWidth() const;
    int getHeight() const;
    std::string getLabel() const;
};

#endif
