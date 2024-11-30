#ifndef MENU_H
#define MENU_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
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
    SDL_Texture* backgroundTexture = nullptr;  // 新增變數儲存背景圖片的紋理
    SDL_Texture* playButtonTexture;  // Play 按鈕圖片
    SDL_Texture* howToPlayButtonTexture;  // How to Play 按鈕圖片
    SDL_Texture* exitButtonTexture;  // Exit 按鈕圖片


public:
    Menu(SDL_Renderer* renderer);
    ~Menu();

    void handleEvent(SDL_Event& e);
    void render(SDL_Renderer* renderer);
};

#endif
