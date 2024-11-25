#ifndef MENU_H
#define MENU_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <vector>
#include "Button.h"
#include "Global.h"

using namespace std;

class Menu {
private:
    vector<Button> buttons;
    TTF_Font* font;
    SDL_Color backgroundColor;

public:
    Menu(SDL_Renderer* renderer);

    void handleEvent(SDL_Event& e);
    void render(SDL_Renderer* renderer);
};

#endif
