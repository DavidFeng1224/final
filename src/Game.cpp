#include "Game.h"
#include "Global.h"
#include "Enemy.h"
#include "Player.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <vector>
#include <cmath>
#include <iostream>
using namespace std;

Game::Game(SDL_Renderer* renderer)
    : player(renderer, 100) {
    if (TTF_Init() == -1) {
        cout << "Failed to initialize SDL_ttf: " << TTF_GetError() << endl;
        return;
    }
    // Initialize enemies
    for (int i = 0; i < 5; ++i) {  // 設定 5 個敵人
        enemies.emplace_back(renderer, SCREEN_WIDTH, SCREEN_HEIGHT);
    }
}

Game::~Game() {
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

void Game::drawGrid(SDL_Renderer* renderer, int grid_size, float colorChangeFactor) {
    Uint8 colorValue = static_cast<Uint8>(190 + 5 * sin(colorChangeFactor));
    SDL_SetRenderDrawColor(renderer, colorValue, colorValue, colorValue, 255);

    for (int x = 0; x <= SCREEN_WIDTH; x += grid_size) {
        SDL_RenderDrawLine(renderer, x, 0, x, SCREEN_HEIGHT);
    }

    for (int y = 0; y <= SCREEN_HEIGHT; y += grid_size) {
        SDL_RenderDrawLine(renderer, 0, y, SCREEN_WIDTH, y);
    }
}

void Game::render(SDL_Renderer* renderer) {
    SDL_SetRenderDrawColor(renderer, 204, 204, 204, 255);  // 淡綠色背景
    SDL_RenderClear(renderer);

    drawGrid(renderer, 40, colorChangeFactor);

    colorChangeFactor += 0.0005f;
    if (colorChangeFactor > 2.0f * M_PI) {
        colorChangeFactor -= 2.0f * M_PI;
    }

    player.render(renderer);  // 繪製玩家

    for (auto& enemy : enemies) {
        enemy.render(renderer);  // 繪製敵人
    }
}
