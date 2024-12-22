#ifndef VICTORY_H
#define VICTORY_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>
#include "Global.h"

class Victory {
public:
    Victory(SDL_Renderer* renderer);
    ~Victory();

    void handleEvent(SDL_Event& e);
    void render(SDL_Renderer* renderer);

private:
    SDL_Color backgroundColor;
    SDL_Texture* backgroundTexture = nullptr;
};

#endif
