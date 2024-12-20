#include "Instruction.h"
#include <iostream>
#include <SDL2/SDL_image.h>

using namespace std;

// Constructor for Instruction
Instruction::Instruction(SDL_Renderer* renderer) {
    // Initialize SDL_ttf
    if (TTF_Init() == -1) {
        cout << "Failed to initialize SDL_ttf: " << TTF_GetError() << endl;
        return;
    }

    // Initialize SDL_image
    if (IMG_Init(IMG_INIT_PNG) != IMG_INIT_PNG) {
        cout << "Failed to initialize SDL_image: " << IMG_GetError() << endl;
        return;
    }

    backgroundColor = {252, 234, 222, 255};

    // 加載背景圖片
    backgroundTexture = IMG_LoadTexture(renderer, "assets/images/Test_Instruction.png");
    if (!backgroundTexture) {
        cout << "Failed to load background texture: " << IMG_GetError() << endl;
    }
}

void Instruction::handleEvent(SDL_Event& e) {
    if (e.type == SDL_KEYDOWN) {
        if (e.key.keysym.sym == SDLK_ESCAPE) {
            extern Gamemode gamemode;
            gamemode = MENU;
            cout << "ESC key pressed. Back to Menu." << endl;
        }
    }
}

void Instruction::render(SDL_Renderer* renderer) {
    SDL_SetRenderDrawColor(renderer, backgroundColor.r, backgroundColor.g, backgroundColor.b, backgroundColor.a);
    SDL_RenderClear(renderer);

    if (backgroundTexture) {
        SDL_RenderCopy(renderer, backgroundTexture, NULL, NULL);
    }
    SDL_RenderPresent(renderer);
}

Instruction::~Instruction() {
    if (backgroundTexture) {
        SDL_DestroyTexture(backgroundTexture);
    }

    IMG_Quit();
}
