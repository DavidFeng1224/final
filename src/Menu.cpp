#include "Menu.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

Menu::Menu(SDL_Renderer* renderer) {
    this->renderer = renderer;
    this->isRunning = true;

    // 初始化按鈕的位置
    startButton = { 300, 270, 200, 50 };  // Start按鈕
    howToPlayButton = { 300, 340, 200, 50 };  // How to Play按鈕
    settingsButton = { 300, 410, 200, 50 };  // Settings按鈕
    exitButton = { 300, 480, 200, 50 };  // Exit按鈕

    // 初始化SDL_ttf庫以顯示文字
    if (TTF_Init() == -1) {
        SDL_Log("TTF_Init failed: %s", TTF_GetError());
    }

    // 加載字型 
    font = TTF_OpenFont("assets/fonts/RussoOne-Regular.ttf", 24);  // 載入字型
    if (!font) {
        SDL_Log("Failed to load font: %s", TTF_GetError());
    }
}

Menu::~Menu() {
    // 釋放字型
    if (font) {
        TTF_CloseFont(font);
    }
    TTF_Quit();
}

void Menu::handleEvent(SDL_Event& e) {
    if (e.type == SDL_MOUSEBUTTONDOWN) {
        int x, y;
        SDL_GetMouseState(&x, &y);
        handleButtonClick(x, y);  // 處理點擊事件
    }
}

void Menu::update() {
    // 更新菜單，若需改變按鈕狀態可以在此處處理
}

void Menu::render() {
    // 設定背景顏色為淺灰色
    SDL_SetRenderDrawColor(renderer, 252, 234, 222, 255);  // RGB(211, 211, 211) = Light Gray
    SDL_RenderClear(renderer);  // 清除上一幀的渲染

    // 渲染按鈕
    renderButton(startButton, "Start", isMouseOverButton(startButton), 3);
    renderButton(howToPlayButton, "How to Play", isMouseOverButton(howToPlayButton), 3);
    renderButton(settingsButton, "Settings", isMouseOverButton(settingsButton), 3);
    renderButton(exitButton, "Exit", isMouseOverButton(exitButton), 3);

    SDL_RenderPresent(renderer);  // 顯示渲染結果
}

// 渲染按鈕並顯示文字，允許設定外框粗細
void Menu::renderButton(SDL_Rect& button, const std::string& text, bool isHovered, int borderThickness) {
    // 設置按鈕背景顏色
    SDL_SetRenderDrawColor(renderer, 255, 243, 190, 255);  // 不知道甚麼顏色
    
    // 渲染按鈕背景
    SDL_RenderFillRect(renderer, &button);

    // 設置外框顏色為藍色
    if (isHovered) {
        SDL_SetRenderDrawColor(renderer, 249, 87, 56, 255);  // 紅色文字
    } else {
        SDL_SetRenderDrawColor(renderer, 82, 91, 118, 255);  // 深藍色
    }

    // 繪製多層藍色邊框
    for (int i = 0; i < borderThickness; ++i) {
        SDL_Rect borderRect = { button.x - i, button.y - i, button.w + 2 * i, button.h + 2 * i };
        SDL_RenderDrawRect(renderer, &borderRect);
    }

    // 設置文字顏色
    SDL_Color textColor;
    if (isHovered) {
        textColor = { 249, 87, 56, 255 };  // 紅色文字
    } else {
        textColor = { 82, 91, 118, 255 };  // 深藍文字
    }

    // 渲染文字
    SDL_Surface* textSurface = TTF_RenderText_Solid(font, text.c_str(), textColor);
    if (!textSurface) {
        SDL_Log("Text surface could not be created: %s", TTF_GetError());
    }
    SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);

    // 設置文字的位置
    SDL_Rect textRect = { button.x + (button.w - textSurface->w) / 2, 
                          button.y + (button.h - textSurface->h) / 2, 
                          textSurface->w, textSurface->h };
    
    // 渲染文字
    SDL_RenderCopy(renderer, textTexture, nullptr, &textRect);

    // 清理
    SDL_FreeSurface(textSurface);
    SDL_DestroyTexture(textTexture);
}

// 處理按鈕點擊事件
void Menu::handleButtonClick(int x, int y) {
    SDL_Point mousePos = { x, y };  // 創建一個SDL_Point物件

    // 檢查點擊是否在按鈕範圍內
    if (SDL_PointInRect(&mousePos, &startButton)) {
        // 開始遊戲
        SDL_Log("Start button clicked!");
    } else if (SDL_PointInRect(&mousePos, &howToPlayButton)) {
        // 顯示遊戲玩法
        SDL_Log("How to Play button clicked!");
    } else if (SDL_PointInRect(&mousePos, &settingsButton)) {
        // 開啟設定
        SDL_Log("Settings button clicked!");
    } else if (SDL_PointInRect(&mousePos, &exitButton)) {
        // 關閉應用程式
        SDL_Log("Exit button clicked!");
        SDL_Quit();  // 清理並退出SDL
        exit(0);     // 結束程式
    }
}

// 判斷滑鼠是否在按鈕上
bool Menu::isMouseOverButton(const SDL_Rect& button) {
    int x, y;
    SDL_GetMouseState(&x, &y);
    SDL_Point mousePos = { x, y };
    return SDL_PointInRect(&mousePos, &button);
}

// 確認程式是否繼續運行
bool Menu::running() const {
    return isRunning;
}
