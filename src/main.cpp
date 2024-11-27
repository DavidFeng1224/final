#define SDL_MAIN_HANDLED  // 告訴 SDL2 不要自動處理 main 函數
#include <SDL2/SDL.h>
#include <iostream>
#include "Menu.h"
#include "Game.h"
#include "Global.h"

Gamemode gamemode = MENU;

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;

using namespace std;

// SDL global variables
SDL_Window* gWindow = NULL;
SDL_Renderer* gRenderer = NULL;

// Starts up SDL and creates window
bool init();

// Frees media and shuts down SDL
void close();

// Render instructions
void renderInstructions(SDL_Renderer* renderer);

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

void renderInstructions(SDL_Renderer* renderer) {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);  // Black background
    SDL_RenderClear(renderer);

    SDL_Rect instructionRect = {200, 200, 800, 500};
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);  // White
    SDL_RenderFillRect(renderer, &instructionRect);

    SDL_RenderPresent(renderer);
}

int main(int argc, char* args[]) {
    if (!init()) {
        printf("Failed to initialize!\n");
        return -1;
    }

    Menu menu(gRenderer);  // Create the menu instance
    Game game(gRenderer);  // Create the menu instance

    bool quit = false;
    SDL_Event e;

    while (!quit) {
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) {
                quit = true;
            }

            if (gamemode == MENU) {
                menu.handleEvent(e);
            }
            switch (gamemode) {
                case MENU:
                    menu.handleEvent(e);
                    break;

                case INGAME:
                    game.handleEvent(e);
                    break;

                case INSTRUCTIONS:
                    break;

                case EXIT:
                    break;

                default:
                    break;
        }
        }

        SDL_SetRenderDrawColor(gRenderer, 0, 0, 0, 255);  // Black background
        SDL_RenderClear(gRenderer);

        switch (gamemode) {
            case MENU:
                menu.render(gRenderer);
                break;

            case INGAME:
                SDL_SetRenderDrawColor(gRenderer, 0, 255, 0, 255);  // Green background
                SDL_RenderClear(gRenderer);
                break;

            case INSTRUCTIONS:
                renderInstructions(gRenderer);
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
