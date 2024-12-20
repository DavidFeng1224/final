#include "Story.h"
#include "Global.h"
#include <iostream>

using namespace std;

Story::Story(SDL_Renderer* renderer)
    : mRenderer(renderer), currentLine(0) {
    font = TTF_OpenFont("assets/fonts/SuperPixel.ttf", 18);

    backgroundTexture = IMG_LoadTexture(mRenderer, "assets/images/Darker_Background_Grass.png");
    Player_Diologue_Texture = IMG_LoadTexture(mRenderer, "assets/images/Player_Dialogue.png");
    if (backgroundTexture == nullptr) {
        cerr << "Failed to load background texture: " << IMG_GetError() << endl;
    }

    dialogue.push_back("Welcome to the world of Course Conqueror! This is a game where you will need to conquer various courses.");
    dialogue.push_back("In this world, you will defeat enemies from your courses, and each enemy represents a challenge.");
    dialogue.push_back("Get ready for the first challenge, and may you succeed in your journey!");

    wrapText(dialogue[currentLine], SCREEN_WIDTH - 100);  // 初始對話換行處理
}

Story::~Story() {
    SDL_DestroyTexture(backgroundTexture);
    SDL_DestroyTexture(Player_Diologue_Texture);
    TTF_CloseFont(font);
}

void Story::handleEvent(SDL_Event& e) {
    if (e.type == SDL_KEYDOWN || e.type == SDL_MOUSEBUTTONDOWN) {
        currentLine++;
        if (currentLine >= dialogue.size()) {
            extern Gamemode gamemode;
            gamemode = INGAME;
        } else {
            wrapText(dialogue[currentLine], SCREEN_WIDTH - 100);  // 更新對話的換行
        }
    }
}

void Story::update(double deltaTime) {
    // 更新劇情邏輯
}

void Story::render(SDL_Renderer* renderer) {
    SDL_RenderCopy(renderer, backgroundTexture, NULL, NULL);

    if (Player_Diologue_Texture) {
        SDL_Rect Player_Diologue_Rect = {100, 200, 600, 300};
        SDL_RenderCopy(renderer, Player_Diologue_Texture, NULL, &Player_Diologue_Rect);
    }

    if (font != nullptr) {
        int yOffset = SCREEN_HEIGHT - 300;  // 將對話框的位置往上調整
        for (const string& line : wrappedText) {
            SDL_Surface* textSurface = TTF_RenderText_Solid(font, line.c_str(), {0, 0, 0});  // 黑色字體
            SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
            SDL_Rect textRect = { 60, yOffset, textSurface->w, textSurface->h };
            SDL_RenderCopy(renderer, textTexture, NULL, &textRect);
            yOffset += textSurface->h;  // 增加y偏移量以顯示下一行
            SDL_FreeSurface(textSurface);
            SDL_DestroyTexture(textTexture);
        }
    }
}

void Story::wrapText(const string& text, int maxWidth) {
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
