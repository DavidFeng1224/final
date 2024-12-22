#include "Story2.h"
#include "Global.h"
#include "AudioManager.h"
#include <SDL2/SDL_mixer.h>
#include <iostream>

using namespace std;

Story2::Story2(SDL_Renderer* renderer)
    : mRenderer(renderer), currentLine(0) {
    font = TTF_OpenFont("assets/fonts/Futura Book font.ttf", 24);

    backgroundTexture = IMG_LoadTexture(mRenderer, "assets/images/Darker_Background_Grass.png");
    Player_Diologue_Texture = IMG_LoadTexture(mRenderer, "assets/images/Player_Dialogue.png");
    Professor_Diologue_Texture = IMG_LoadTexture(mRenderer, "assets/images/Professor_Dialogue.png");
    if (backgroundTexture == nullptr) {
        cerr << "Failed to load background texture: " << IMG_GetError() << endl;
    }

    dialogue.push_back("You seem quite skilled!");
    dialogue.push_back("Not really, just a little trick.");
    dialogue.push_back("It looks like I'll have to show you the  power of NTUEE");
    dialogue.push_back("What?");
    dialogue.push_back("Your next opponent will be... ME!");
    dialogue.push_back("......");

    wrapText(dialogue[currentLine], SCREEN_WIDTH - 400);  // 初始對話換行處理
}

Story2::~Story2() {
    SDL_DestroyTexture(backgroundTexture);
    SDL_DestroyTexture(Player_Diologue_Texture);
    SDL_DestroyTexture(Professor_Diologue_Texture);
    TTF_CloseFont(font);
}

void Story2::handleEvent(SDL_Event& e) {
    if (e.type == SDL_KEYDOWN || e.type == SDL_MOUSEBUTTONDOWN) {
        currentLine++;
        if (currentLine >= dialogue.size()) {
            extern Gamemode gamemode;
            gamemode = FINALGAME;
            playMusic("assets/sounds/Final_BGM.mp3");
        } else {
            wrapText(dialogue[currentLine], SCREEN_WIDTH - 400);  // 更新對話的換行
        }
    }
}

void Story2::update(double deltaTime) {
    // 更新劇情邏輯
}

void Story2::render(SDL_Renderer* renderer) {
    SDL_RenderCopy(renderer, backgroundTexture, NULL, NULL);

    if(currentLine % 2 == 1){
        if (Player_Diologue_Texture) {
            SDL_Rect Player_Diologue_Rect = {50, 200, 700, 350};
            SDL_RenderCopy(renderer, Player_Diologue_Texture, NULL, &Player_Diologue_Rect);
        }
        if (font != nullptr) {
            int yOffset = SCREEN_HEIGHT - 180;
            for (const string& line : wrappedText) {
                SDL_Surface* textSurface = TTF_RenderText_Solid(font, line.c_str(), {184, 61, 186});
                SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
                SDL_Rect textRect = { 250, yOffset, textSurface->w, textSurface->h };
                SDL_RenderCopy(renderer, textTexture, NULL, &textRect);
                yOffset += textSurface->h;
                SDL_FreeSurface(textSurface);
                SDL_DestroyTexture(textTexture);
            }
        }
    }else{
        if (Professor_Diologue_Texture) {
            SDL_Rect Professor_Diologue_Rect = {50, 200, 700, 350};
            SDL_RenderCopy(renderer, Professor_Diologue_Texture, NULL, &Professor_Diologue_Rect);
        }
        if (font != nullptr) {
            int yOffset = SCREEN_HEIGHT - 180;
            for (const string& line : wrappedText) {
                SDL_Surface* textSurface = TTF_RenderText_Solid(font, line.c_str(), {126, 118, 46});
                SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
                SDL_Rect textRect = { 150, yOffset, textSurface->w, textSurface->h };
                SDL_RenderCopy(renderer, textTexture, NULL, &textRect);
                yOffset += textSurface->h;
                SDL_FreeSurface(textSurface);
                SDL_DestroyTexture(textTexture);
            }
        }
    }
}

void Story2::wrapText(const string& text, int maxWidth) {
    wrappedText.clear();  // 清除之前的換行內容
    string currentLine = "";
    for (size_t i = 0; i < text.length(); ++i) {
        currentLine += text[i];

        // 測量當前行的寬度
        SDL_Surface* surface = TTF_RenderText_Solid(font, currentLine.c_str(), {255, 255, 255});
        int width = surface->w;
        SDL_FreeSurface(surface);

        // 如果當前行的寬度超過最大寬度，進行換行
        if (width > maxWidth) {
            // 將當前行加入 wrappedText，並清空 currentLine
            wrappedText.push_back(currentLine.substr(0, currentLine.length() - 1));
            currentLine = currentLine.substr(currentLine.length() - 1);  // 保留最後一個字符
        }
    }

    if (!currentLine.empty()) {
        wrappedText.push_back(currentLine);  // 添加最後一行
    }
}
