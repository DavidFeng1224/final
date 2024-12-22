#define SDL_MAIN_HANDLED  // 告訴 SDL2 不要自動處理 main 函數
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <iostream>
#include "AudioManager.h"
#include "Menu.h"
#include "Game.h"
#include "FinalGame.h"
#include "Instruction.h"
#include "Story1.h"
#include "Story2.h"
#include "Victory.h"
#include "Gameover.h"
#include "Global.h"
#include "Player.h"

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

    // Initialize SDL_image
    if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG)) {
        printf("SDL_image could not initialize! IMG Error: %s\n", IMG_GetError());
        return false;
    }
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0) {
        std::cerr << "Failed to initialize SDL: " << SDL_GetError() << std::endl;
        return -1;
    }

    return true;
}

void close() {
    SDL_DestroyRenderer(gRenderer);
    SDL_DestroyWindow(gWindow);
    IMG_Quit();
    cleanupAudio();  // 清理音頻系統
    SDL_Quit();
}

int main(int argc, char* args[]) {
    if (!init()) {
        printf("Failed to initialize!\n");
        return -1;
    }
    initializeAudio();  

    Menu menu(gRenderer);
    Game game(gRenderer);
    FinalGame finalgame(gRenderer);
    Instruction instruction(gRenderer);
    Story1 story1(gRenderer);
    Story2 story2(gRenderer);
    Victory victory(gRenderer);
    Gameover gameover(gRenderer);
    // Create a Player object
    Player player(gRenderer, 200.0);  // Pass renderer and speed to the Player constructor

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

                case FINALGAME:
                    finalgame.handleEvent(e);
                    break;

                case INSTRUCTIONS:
                    instruction.handleEvent(e);
                    break;

                case STORY1:
                    story1.handleEvent(e);
                    break;

                case STORY2:
                    story2.handleEvent(e);
                    break;

                case VICTORY:
                    victory.handleEvent(e);
                    break;

                case GAMEOVER:
                    gameover.handleEvent(e);
                    break;

                case EXIT:
                    break;

                default:
                    break;
            }
        }


        SDL_SetRenderDrawColor(gRenderer, 0, 0, 0, 255);
        SDL_RenderClear(gRenderer);

        switch (gamemode) {
            case MENU:
                menu.update(deltaTime);  // 使用deltaTime來更新遊戲
                menu.render(gRenderer);
                break;

            case INGAME:
                game.update(deltaTime);  // 使用deltaTime來更新遊戲
                game.render(gRenderer);  // 渲染遊戲
                break;

            case FINALGAME:
                finalgame.update(deltaTime);  // 使用deltaTime來更新遊戲
                finalgame.render(gRenderer);  // 渲染遊戲
                break;

            case INSTRUCTIONS:
                instruction.render(gRenderer);
                break;

            case STORY1:
                story1.render(gRenderer);
                break;

            case STORY2:
                story2.render(gRenderer);
                break;

            case VICTORY:
                victory.render(gRenderer);
                break;

            case GAMEOVER:
                gameover.render(gRenderer);
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
