#ifndef MENU_H
#define MENU_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_ttf.h>
#include <vector>
#include "Button.h"
#include "Global.h"

using namespace std;

class Menu {
private:
    vector<Button> buttons;
    TTF_Font* font;
    SDL_Color backgroundColor;
    SDL_Texture* backgroundTexture = nullptr;       // 背景圖片紋理
    SDL_Texture* playButtonTexture = nullptr;       // Play 按鈕圖片
    SDL_Texture* howToPlayButtonTexture = nullptr;  // How to Play 按鈕圖片
    SDL_Texture* exitButtonTexture = nullptr;       // Exit 按鈕圖片
    Mix_Music* backgroundMusic;

    // 標題相關
    SDL_Texture* titleTexture = nullptr;  // 標題圖片紋理
    SDL_Rect titleRect;                   // 標題位置和大小
    float scaleFactor;                    // 縮放因子
    float scaleSpeed;                     // 縮放速度
    bool isScalingUp;                     // 紀錄是否正在放大

public:
    Menu(SDL_Renderer* renderer);
    ~Menu();

    void handleEvent(SDL_Event& e);
    void update(float deltaTime); // 更新動畫的函式
    void render(SDL_Renderer* renderer);
};

#endif
