#ifndef GAME_H
#define GAME_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <vector>
#include "Button.h"
#include "Global.h"

using namespace std;

class Game {
private:
    vector<Button> buttons;
    TTF_Font* font;
    SDL_Color backgroundColor;

public:
    Game(SDL_Renderer* renderer);

    void handleEvent(SDL_Event& e);
    void render(SDL_Renderer* renderer);
};

#endif
