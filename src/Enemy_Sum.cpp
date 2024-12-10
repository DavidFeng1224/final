#include "Enemy_Sum.h"
#include "Global.h"
#include <SDL2/SDL_image.h>
#include <cstdlib>
#include <ctime>
#include <iostream>

Enemy_Sum::Enemy_Sum(SDL_Renderer *renderer)
    : BaseEnemy(renderer) {
    std::srand(static_cast<unsigned>(std::time(0)));

    mPosX = static_cast<float>(std::rand() % SCREEN_WIDTH);
    mPosY = static_cast<float>(std::rand() % SCREEN_HEIGHT);
    mSpeed = 1.0f;   // 初始速度
    mHP = 120;       // 血量
    mDamage = 10;    // 攻擊力
    mRadius = 25.0f; // 大小半徑

    // 加載圖片
    Texture = IMG_LoadTexture(renderer, "assets/images/Enemy_Sum.png");
    if (!Texture) {
        std::cerr << "Failed to load Enemy_Sum texture: " << IMG_GetError() << std::endl;
    }
}

Enemy_Sum::~Enemy_Sum() {
    if (Texture) {
        SDL_DestroyTexture(Texture);
    }
}

void Enemy_Sum::update(double deltaTime) {
    mPosX += mSpeed * deltaTime * 100;
    mPosY += mSpeed * deltaTime * 100;

    // 邊界檢查並反彈
    if (mPosX - mRadius < 0 || mPosX + mRadius > SCREEN_WIDTH) {
        mSpeed = -mSpeed;
    }
    if (mPosY - mRadius < 0 || mPosY + mRadius > SCREEN_HEIGHT) {
        mSpeed = -mSpeed;
    }
}

void Enemy_Sum::render(SDL_Renderer *renderer) {
    BaseEnemy::render(renderer); 
    if (Texture) {
        SDL_Rect rect = {static_cast<int>(mPosX - mRadius), static_cast<int>(mPosY - mRadius),
                         static_cast<int>(mRadius * 2), static_cast<int>(mRadius * 2)};
        SDL_RenderCopy(renderer, Texture, NULL, &rect);
    } else {
        int radius = static_cast<int>(mRadius);
        SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
        for (int w = 0; w < radius * 2; w++) {
            for (int h = 0; h < radius * 2; h++) {
                int dx = radius - w;
                int dy = radius - h;
                if ((dx * dx + dy * dy) <= (radius * radius)) {
                    SDL_RenderDrawPoint(renderer, static_cast<int>(mPosX) + dx, static_cast<int>(mPosY) + dy);
                }
            }
        }
    }
}
