#include "Game.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <vector>
#include "Global.h"
#include "Enemy.h"
#include <cmath>  // For sin() to create smooth oscillation
#include <iostream>  // For outputting to console
using namespace std;

// Constructor
Game::Game(SDL_Renderer* renderer){
    // Initialize SDL_ttf (if you're using fonts)
    if (TTF_Init() == -1) {
        cout << "Failed to initialize SDL_ttf: " << TTF_GetError() << endl;
        return;
    }

    // Other initialization tasks (e.g., loading resources, setting up game state)
}

// Destructor
Game::~Game() {
    // Clean up resources, like closing SDL_ttf
    TTF_Quit();
}

// Handle events (keyboard, mouse, etc.)
void Game::handleEvent(SDL_Event& e) {
    // Handle quit event (e.g., pressing the X button to close the window)
    if (e.type == SDL_QUIT) {
        extern Gamemode gamemode;
        gamemode = EXIT;
        cout << "Quit event detected. Exiting game." << endl;
    }

    // Handle keydown events (e.g., ESC key to quit)
    if (e.type == SDL_KEYDOWN) {
        if (e.key.keysym.sym == SDLK_ESCAPE) {
            extern Gamemode gamemode;
            gamemode = EXIT;
            cout << "ESC key pressed. Exiting game." << endl;
        }
    }

    // Add more event handling logic here (e.g., mouse clicks, other key presses)
}

void Game::drawGrid(SDL_Renderer* renderer, int window_width, int window_height, int grid_size, float colorChangeFactor) {
    // 計算顏色，基於 colorChangeFactor 來進行漸變
    Uint8 colorValue = static_cast<Uint8>(190 + 5 * sin(colorChangeFactor));  // 顏色變化範圍：100 到 200

    SDL_SetRenderDrawColor(renderer, colorValue, colorValue, colorValue, 255); // 設定顏色

    // 畫垂直線
    for (int x = 0; x <= window_width; x += grid_size) {
        SDL_RenderDrawLine(renderer, x, 0, x, window_height);
    }

    // 畫水平線
    for (int y = 0; y <= window_height; y += grid_size) {
        SDL_RenderDrawLine(renderer, 0, y, window_width, y);
    }
}

void Game::render(SDL_Renderer* renderer) {
    // 設定背景顏色為很淡的綠色
    SDL_SetRenderDrawColor(renderer, 204, 204, 204, 255);  // 淡綠色背景
    SDL_RenderClear(renderer);  // 填充背景

    // 在這裡繪製格線背景
    drawGrid(renderer, SCREEN_WIDTH, SCREEN_HEIGHT, 40, colorChangeFactor);

    // 更新顏色變化因子
    colorChangeFactor += 0.001f;  // 控制顏色變化的速度
    if (colorChangeFactor > 2.0f * M_PI) {  // 超過一個週期後重置
        colorChangeFactor -= 2.0f * M_PI;
    }

    // 渲染遊戲畫面中的其他元素
    // 這邊可以繼續繪製遊戲中的其他內容，例如角色、物件等
}
