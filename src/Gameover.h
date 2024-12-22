#ifndef GAMEOVER_H
#define GAMEOVER_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>
#include "Global.h"

class Gameover {
public:
    Gameover(SDL_Renderer* renderer);
    ~Gameover();

    void handleEvent(SDL_Event& e);
    void render(SDL_Renderer* renderer);

private:
    SDL_Color backgroundColor;
    SDL_Texture* backgroundTexture = nullptr;
};

#endif
