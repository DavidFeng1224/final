#include "Enemy_Integral.h"
#include <cmath>
#include <SDL2/SDL_image.h>
#include <iostream>

Enemy_Integral::Enemy_Integral(SDL_Renderer *renderer, Player *player)
    : BaseEnemy(renderer), target(player) {
    mPosX = 200.0f;  // 初始位置 X
    mPosY = 200.0f;  // 初始位置 Y
    mSpeed = 80.0f;  // 速度
    mHP = 100;       // 血量
    mDamage = 15;    // 攻擊力
    mRadius = 25.0f; // 大小半徑

    // 加載圖片
    Texture = IMG_LoadTexture(renderer, "assets/images/Enemy_Integral.png");
    if (!Texture) {
        std::cerr << "Failed to load Enemy_Integral texture: " << IMG_GetError() << std::endl;
    }
}

Enemy_Integral::~Enemy_Integral() {
    if (Texture) {
        SDL_DestroyTexture(Texture);
    }
}

void Enemy_Integral::update(double deltaTime) {
    if (!target) return;

    // 計算方向向量
    float dx = target->getX() - mPosX;
    float dy = target->getY() - mPosY;

    float distance = std::sqrt(dx * dx + dy * dy);
    if (distance > 0) {
        dx /= distance;
        dy /= distance;

        // 更新位置
        mPosX += dx * mSpeed * deltaTime;
        mPosY += dy * mSpeed * deltaTime;
    }
}

void Enemy_Integral::render(SDL_Renderer *renderer) {
    if (Texture) {
        SDL_Rect rect = {static_cast<int>(mPosX - mRadius), static_cast<int>(mPosY - mRadius),
                         static_cast<int>(mRadius * 2), static_cast<int>(mRadius * 2)};
        SDL_RenderCopy(renderer, Texture, NULL, &rect);
    } else {
        int radius = static_cast<int>(mRadius);
        SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);
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
