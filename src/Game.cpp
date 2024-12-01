#include <iostream>
#include <vector>
#include <cmath>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>
#include "Game.h"
#include "Global.h"
#include "Enemy.h"
#include "Player.h"

using namespace std;

Game::Game(SDL_Renderer* renderer)
    : player(renderer, 100) {
    if (TTF_Init() == -1) {
        cout << "Failed to initialize SDL_ttf: " << TTF_GetError() << endl;
        return;
    }

    if (IMG_Init(IMG_INIT_PNG) != IMG_INIT_PNG) {
        cout << "Failed to initialize SDL_image: " << IMG_GetError() << endl;
        return;
    }

    backgroundTexture = IMG_LoadTexture(renderer, "assets/images/Background_Grass.jpeg");  
    if (!backgroundTexture) {
        cout << "Failed to load background texture: " << IMG_GetError() << endl;
    }

    // Initialize enemies
    for (int i = 0; i < 5; ++i) {  // 設定 5 個敵人
        enemies.emplace_back(renderer);
    }
}

Game::~Game() {
    if (backgroundTexture) {
        SDL_DestroyTexture(backgroundTexture);
    }
    IMG_Quit();

    TTF_Quit();
}

void Game::handleEvent(SDL_Event& e) {
    if (e.type == SDL_QUIT) {
        extern Gamemode gamemode;
        gamemode = EXIT;
        cout << "Quit event detected. Exiting game." << endl;
    }

    if (e.type == SDL_KEYDOWN) {
        if (e.key.keysym.sym == SDLK_ESCAPE) {
            extern Gamemode gamemode;
            gamemode = EXIT;
            cout << "ESC key pressed. Exiting game." << endl;
        }
    }

    player.handleEvent(e);
}

void Game::update(double deltaTime) {
    player.update(deltaTime);

    for (auto& enemy : enemies) {
        enemy.update(deltaTime);
    }
}

void Game::render(SDL_Renderer* renderer) {
    SDL_SetRenderDrawColor(renderer, 204, 204, 204, 255);  // 淡綠色背景
    SDL_RenderClear(renderer);

    if (backgroundTexture) {
        SDL_RenderCopy(renderer, backgroundTexture, NULL, NULL);  // 渲染背景
    }

    player.render(renderer);  // 繪製玩家

    for (auto& enemy : enemies) {
        enemy.render(renderer);  // 繪製敵人
    }
}
