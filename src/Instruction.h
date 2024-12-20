#ifndef INSTRUCTION_H
#define INSTRUCTION_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>
#include "Global.h"

class Instruction {
public:
    Instruction(SDL_Renderer* renderer);
    ~Instruction();

    void handleEvent(SDL_Event& e);
    void render(SDL_Renderer* renderer);

private:
    SDL_Color backgroundColor;
    SDL_Texture* backgroundTexture = nullptr;
};

#endif
