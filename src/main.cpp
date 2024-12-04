#define SDL_MAIN_HANDLED  // 告訴 SDL2 不要自動處理 main 函數
#include <SDL2/SDL.h>
#include <iostream>
#include "Menu.h"
#include "Game.h"
#include "Instruction.h"
#include "Global.h"



Gamemode gamemode = MENU;

using namespace std;

// SDL global variables
SDL_Window* gWindow = NULL;
SDL_Renderer* gRenderer = NULL;

bool init();
void close();

bool init() {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("SDL could not initialize! SDL Error: %s\n", SDL_GetError());
        return false;
    }

    gWindow = SDL_CreateWindow("COURSE CONQUEROR", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (gWindow == NULL) {
        printf("Window could not be created! SDL Error: %s\n", SDL_GetError());
        return false;
    }

    gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED);
    if (gRenderer == NULL) {
        printf("Renderer could not be created! SDL Error: %s\n", SDL_GetError());
        return false;
    }

    return true;
}

void close() {
    SDL_DestroyRenderer(gRenderer);
    SDL_DestroyWindow(gWindow);
    SDL_Quit();
}

int main(int argc, char* args[]) {
    if (!init()) {
        printf("Failed to initialize!\n");
        return -1;
    }

    Menu menu(gRenderer);
    Game game(gRenderer);
    Instruction instruction(gRenderer);

    bool quit = false;
    SDL_Event e;

    Uint32 lastTime = SDL_GetTicks();  // 記錄上次更新的時間

    while (!quit) {
        Uint32 currentTime = SDL_GetTicks();
        double deltaTime = (currentTime - lastTime) / 1000.0;  // 計算每幀的時間差，轉換為秒
        lastTime = currentTime;  // 更新lastTime為當前時間

        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) {
                quit = true;
            }
            switch (gamemode) {
                case MENU:
                    menu.handleEvent(e);
                    break;

                case INGAME:
                    game.handleEvent(e);
                    break;

                case INSTRUCTIONS:
                    instruction.handleEvent(e);
                    break;

                case EXIT:
                    break;

                default:
                    break;
            }
        }

        game.update(deltaTime);  // 使用deltaTime來更新遊戲

        SDL_SetRenderDrawColor(gRenderer, 0, 0, 0, 255);
        SDL_RenderClear(gRenderer);

        switch (gamemode) {
            case MENU:
                menu.render(gRenderer);
                break;

            case INGAME:
                game.render(gRenderer);  // 渲染遊戲
                break;

            case INSTRUCTIONS:
                instruction.render(gRenderer);
                break;

            case EXIT:
                quit = true;
                break;

            default:
                break;
        }

        SDL_RenderPresent(gRenderer);
    }

    close();
    return 0;
}
