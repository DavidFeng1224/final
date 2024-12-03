#include <iostream>
#include <vector>
#include <cmath>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>
#include "Game.h"
#include "Global.h"
#include "Enemy.h"
#include "EnemyFollow.h"
#include "Player.h"

using namespace std;

Game::Game(SDL_Renderer* renderer) 
    : player(renderer, 100) {
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

    // Load background texture
    backgroundTexture = IMG_LoadTexture(renderer, "assets/images/Background_Grass.jpeg");
    if (!backgroundTexture) {
        cout << "Failed to load background texture: " << IMG_GetError() << endl;
    }

    // Initialize random enemies
    for (int i = 0; i < 3; ++i) {
        enemies.push_back(static_cast<BaseEnemy*>(new Enemy(renderer)));  // 確保轉換為 BaseEnemy 指針
  
    }

    // Initialize chasing enemies
    for (int i = 0; i < 2; ++i) {
        enemies.push_back(new EnemyFollow(renderer, &player));  // Add chasing enemy
    }
}

Game::~Game() {
    // Clean up dynamically allocated enemies
    for (BaseEnemy* enemy : enemies) {
        delete enemy;
    }
    enemies.clear();

    // Destroy background texture
    if (backgroundTexture) {
        SDL_DestroyTexture(backgroundTexture);
    }

    TTF_Quit();
    IMG_Quit();
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

    // Update all enemies
    for (BaseEnemy* enemy : enemies) {
        enemy->update(deltaTime);
    }
}

void Game::render(SDL_Renderer* renderer) {
    SDL_SetRenderDrawColor(renderer, 204, 204, 204, 255);  // Background color
    SDL_RenderClear(renderer);

    // Render background
    if (backgroundTexture) {
        SDL_RenderCopy(renderer, backgroundTexture, NULL, NULL);
    }

    // Render player
    player.render(renderer);

    // Render all enemies
    for (BaseEnemy* enemy : enemies) {
        enemy->render(renderer);
    }
}
