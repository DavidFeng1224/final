#include "Enemy_Hsieh.h"
#include <SDL2/SDL_image.h>
#include <iostream>
#include <cmath>

Enemy_Hsieh::Enemy_Hsieh(SDL_Renderer* renderer, Uint32 spawnTime, Player* player)
    : BaseEnemy(renderer), spawnTime(spawnTime), target(player), speedMultiplier(2.0f), isBoosted(false), boostStartTime(0), boostDuration(1000) {
    // 初始化屬性
    mPosX = static_cast<float>(std::rand() % SCREEN_WIDTH);
    mPosY = static_cast<float>(std::rand() % SCREEN_HEIGHT);
    mSpeed = 150.0f;  // 基礎速度
    mHP = 200;        // 高血量
    mDamage = 50;     // 高傷害
    mRadius = 30.0f;  // 大小半徑

    // 加載圖片
    Texture = IMG_LoadTexture(renderer, "assets/images/Enemy_Hsieh.png");
    if (!Texture) {
        std::cerr << "Failed to load Enemy_Hsieh texture: " << IMG_GetError() << std::endl;
    }

    mHealthBar.setHealth(mHP, 200);
}

Enemy_Hsieh::~Enemy_Hsieh() {
    if (Texture) {
        SDL_DestroyTexture(Texture);
    }
}

void Enemy_Hsieh::update(double deltaTime) {
    if (!target) return;

    // 計算方向向量
    float dx = target->getX() - mPosX;
    float dy = target->getY() - mPosY;
    float distance = std::sqrt(dx * dx + dy * dy);

    if (distance > 0) {
        dx /= distance;
        dy /= distance;

        // 移動敵人
        float effectiveSpeed = isBoosted ? mSpeed * speedMultiplier : mSpeed;
        mPosX += dx * effectiveSpeed * deltaTime;
        mPosY += dy * effectiveSpeed * deltaTime;
    }

    // 檢查加速狀態
    Uint32 currentTime = SDL_GetTicks();
    if (isBoosted && currentTime - boostStartTime > boostDuration) {
        isBoosted = false;
    }

    // 檢查與玩家的碰撞
    checkCollisionWithPlayer();
}

void Enemy_Hsieh::render(SDL_Renderer* renderer) {
    BaseEnemy::render(renderer);  // 繪製基本敵人邏輯

    if (Texture) {
        SDL_Rect rect = {static_cast<int>(mPosX - mRadius), static_cast<int>(mPosY - mRadius),
                         static_cast<int>(mRadius * 2), static_cast<int>(mRadius * 2)};
        SDL_RenderCopy(renderer, Texture, NULL, &rect);
    }
}

void Enemy_Hsieh::checkCollisionWithPlayer() {
    if (!target) return;

    // 計算與玩家的距離
    float dx = target->getX() - mPosX;
    float dy = target->getY() - mPosY;
    float distance = std::sqrt(dx * dx + dy * dy);

    if (distance < mRadius + target->getRadius()) {
        // 撞擊後造成傷害並加速
        target->takeDamage(mDamage);
        isBoosted = true;
        boostStartTime = SDL_GetTicks();
        std::cout << "Enemy_Hsieh collided with the player!" << std::endl;
    }
}
