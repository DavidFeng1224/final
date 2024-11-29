#ifndef INSTRUCTION_H
#define INSTRUCTION_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>
#include "Global.h"

class Instruction {
public:
    Instruction(SDL_Renderer* renderer);  // Constructor
    ~Instruction();  // Destructor

    void handleEvent(SDL_Event& e);
    void render(SDL_Renderer* renderer);

private:
    SDL_Color backgroundColor;
    SDL_Texture* backgroundTexture = nullptr;  // 新增變數儲存背景圖片的紋理

    // 其他成員變數或函數
};

#endif
