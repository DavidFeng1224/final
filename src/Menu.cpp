#include "Menu.h"
#include "Button.h"
#include "Global.h"
#include "AudioManager.h"
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_image.h>
#include <iostream>

using namespace std;

Menu::Menu(SDL_Renderer* renderer) 
     : scaleFactor(1.0f), scaleSpeed(0.2f), isScalingUp(true){

    if (TTF_Init() == -1) {
        cout << "Failed to initialize SDL_ttf: " << TTF_GetError() << endl;
        return;
    }

    if (IMG_Init(IMG_INIT_PNG) != IMG_INIT_PNG) {
        cout << "Failed to initialize SDL_image: " << IMG_GetError() << endl;
        return;
    }

    backgroundTexture = IMG_LoadTexture(renderer, "assets/images/Menu_Background.jpeg");  
    playButtonTexture = IMG_LoadTexture(renderer, "assets/images/Button_PLAY.png");
    howToPlayButtonTexture = IMG_LoadTexture(renderer, "assets/images/Button_HOWTOPLAY.png");
    exitButtonTexture = IMG_LoadTexture(renderer, "assets/images/Button_EXIT.png");
    titleTexture = IMG_LoadTexture(renderer, "assets/images/Menu_Title.png");

    if (!backgroundTexture || !playButtonTexture || !howToPlayButtonTexture || !exitButtonTexture || !titleTexture) {
        cout << "Failed to load images: " << IMG_GetError() << endl;
    }
    playMusic("assets/sounds/Menu_BGM.mp3");

    // 定義按鈕顏色
    SDL_Color textColor = {249, 87, 56, 255};   // 黑色文字
    SDL_Color buttonColor = {0, 0, 0, 0};  // 背景顏色透明
    SDL_Color hoverColor = {0, 0, 0, 0};   // 懸停顏色透明
    SDL_Color borderColor = {0, 0, 0, 0};  // 邊框顏色透明
    int borderThickness = 0;

    // 加載字型
    font = TTF_OpenFont("assets/fonts/RussoOne-Regular.ttf", 24);
    if (!font) {
        cout << "Failed to load font: " << TTF_GetError() << endl;
        return;
    }

    // 設定背景顏色
    backgroundColor = {252, 234, 222, 255}; // 淺色背景

    // 創建按鈕
    Button playButton("Play", 310, 305, 180, 65, buttonColor, textColor, hoverColor, borderColor, borderThickness, font);
    Button howToPlayButton("How to Play", 310, 395, 180, 65, buttonColor, textColor, hoverColor, borderColor, borderThickness, font);
    Button exitButton("Exit", 310, 485, 180, 65, buttonColor, textColor, hoverColor, borderColor, borderThickness, font);

    // 設定按鈕的點擊事件
    playButton.setOnClick([]() {
        extern Gamemode gamemode;  // 引用全域變數
        gamemode = STORY;
        playMusic("assets/sounds/Story_BGM.mp3");
        cout << "Play button clicked! Transitioning to STORY." << endl;
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
void Menu::handleEvent(SDL_Event& e) {
    for (auto& button : buttons) {
        button.handleEvent(e);
    }
}

void Menu::update(float deltaTime) {
    // 更新標題縮放邏輯
    if (isScalingUp) {
        scaleFactor += scaleSpeed * deltaTime;
        if (scaleFactor >= 1.2f) {
            scaleFactor = 1.2f;
            isScalingUp = false;
        }
    } else {
        scaleFactor -= scaleSpeed * deltaTime;
        if (scaleFactor <= 1.0f) {
            scaleFactor = 1.0f;
            isScalingUp = true;
        }
    }

    // 更新標題位置和大小
    titleRect.w = static_cast<int>(382 * scaleFactor);
    titleRect.h = static_cast<int>(150 * scaleFactor);
    titleRect.x = SCREEN_WIDTH / 2 - titleRect.w / 2;
    titleRect.y = 50;
}

// 渲染菜單畫面（背景和按鈕）
void Menu::render(SDL_Renderer* renderer) {
    // 設定背景顏色
    SDL_SetRenderDrawColor(renderer, backgroundColor.r, backgroundColor.g, backgroundColor.b, backgroundColor.a);
    SDL_RenderClear(renderer);  // 清空畫面並填充背景顏色

    if (backgroundTexture) {
        SDL_RenderCopy(renderer, backgroundTexture, NULL, NULL);  // 渲染背景
    }

    for (auto& button : buttons) {
        button.render(renderer);
    }

    // 渲染按鈕圖片，根據按鈕的位置渲染
    if (playButtonTexture) {
        if(buttons[0].getIsHovered()){
            SDL_Rect playButtonRect = {292, 295, 216, 85};
            SDL_RenderCopy(renderer, playButtonTexture, NULL, &playButtonRect);
        }else{
            SDL_Rect playButtonRect = {300, 300, 200, 75};
            SDL_RenderCopy(renderer, playButtonTexture, NULL, &playButtonRect);
        }
    }
    if (howToPlayButtonTexture) {
        if(buttons[1].getIsHovered()){
            SDL_Rect howToPlayButtonRect = {292, 385, 216, 85};
            SDL_RenderCopy(renderer, howToPlayButtonTexture, NULL, &howToPlayButtonRect);
        }else{
            SDL_Rect howToPlayButtonRect = {300, 390, 200, 75};
            SDL_RenderCopy(renderer, howToPlayButtonTexture, NULL, &howToPlayButtonRect);
        }        
    }
    if (exitButtonTexture) {
        if(buttons[2].getIsHovered()){
            SDL_Rect exitButtonRect = {292, 475, 216, 85};
            SDL_RenderCopy(renderer, exitButtonTexture, NULL, &exitButtonRect);
        }else{
            SDL_Rect exitButtonRect = {300, 480, 200, 75};
            SDL_RenderCopy(renderer, exitButtonTexture, NULL, &exitButtonRect);
        }
    }
    if (titleTexture) {
        SDL_RenderCopy(renderer, titleTexture, NULL, &titleRect);
    }

    SDL_RenderPresent(renderer);  // 更新顯示
}


Menu::~Menu() {
    if (backgroundTexture) {
        SDL_DestroyTexture(backgroundTexture);
    }
    if (playButtonTexture) {
        SDL_DestroyTexture(playButtonTexture);
    }
    if (howToPlayButtonTexture) {
        SDL_DestroyTexture(howToPlayButtonTexture);
    }
    if (exitButtonTexture) {
        SDL_DestroyTexture(exitButtonTexture);
    }
    if (backgroundMusic) {
        Mix_FreeMusic(backgroundMusic);
    }
    Mix_CloseAudio();
    IMG_Quit();
}
