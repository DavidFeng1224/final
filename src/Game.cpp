#include "Game.h"
#include "Button.h"
#include "Global.h"
#include <iostream>

using namespace std;

// Constructor for Menu
Game::Game(SDL_Renderer* renderer) {
    // Initialize SDL_ttf
    if (TTF_Init() == -1) {
        cout << "Failed to initialize SDL_ttf: " << TTF_GetError() << endl;
        return;  // Return early if initialization fails
    }

    // Define button colors
    SDL_Color buttonColor = {255, 243, 190, 255};  // 淺黃色
    SDL_Color textColor = {249, 87, 56, 255};          // 黑色文字
    SDL_Color hoverColor = {255, 200, 100, 255};   // 滑鼠懸停顏色
    SDL_Color borderColor = {0, 0, 0, 255};        // 黑色邊框
    int borderThickness = 1;

    // Load font
    font = TTF_OpenFont("assets/fonts/RussoOne-Regular.ttf", 24);
    if (!font) {
        cout << "Failed to load font: " << TTF_GetError() << endl;
        return;  // Return early if the font fails to load
    }

    // Background color for the menu
    backgroundColor = {252, 234, 222, 255}; // 淺色背景

    // Create buttons
    Button playButton("Play", 300, 270, 200, 50, buttonColor, textColor, hoverColor, borderColor, borderThickness, font);
    Button howToPlayButton("How to Play", 300, 340, 200, 50, buttonColor, textColor, hoverColor, borderColor, borderThickness, font);
    Button exitButton("Exit", 300, 410, 200, 50, buttonColor, textColor, hoverColor, borderColor, borderThickness, font);

    // 設定按鈕的點擊事件
    playButton.setOnClick([]() {
        extern Gamemode gamemode;  // 引用全域變數
        gamemode = INGAME;
        cout << "Play button clicked! Transitioning to INGAME." << endl;
    });

    howToPlayButton.setOnClick([]() {
        extern Gamemode gamemode;  // 引用全域變數
        gamemode = INSTRUCTIONS;
        cout << "How to Play button clicked! Transitioning to INSTRUCTIONS." << endl;
    });

    exitButton.setOnClick([]() {
        extern Gamemode gamemode;  // 引用全域變數
        gamemode = EXIT;
        cout << "Exit button clicked! Exiting game." << endl;
    });

    // 將按鈕加入按鈕向量
    buttons.push_back(playButton);
    buttons.push_back(howToPlayButton);
    buttons.push_back(exitButton);
}

// 處理事件（例如按鈕點擊）
void Game::handleEvent(SDL_Event& e) {
    for (auto& button : buttons) {
        button.handleEvent(e);
    }
}

// 渲染菜單畫面（背景和按鈕）
void Game::render(SDL_Renderer* renderer) {
    // 設定背景顏色
    SDL_SetRenderDrawColor(renderer, backgroundColor.r, backgroundColor.g, backgroundColor.b, backgroundColor.a);
    SDL_RenderClear(renderer);  // 清空畫面並填充背景顏色

    // 渲染每個按鈕
    for (auto& button : buttons) {
        button.render(renderer);
    }

    SDL_RenderPresent(renderer);  // 更新顯示
}

