#include "Enemy_Hsieh.h"
#include <SDL2/SDL_image.h>
#include <iostream>
#include <cmath>
#include <cstdlib>

// 構造函式
Enemy_Hsieh::Enemy_Hsieh(SDL_Renderer* renderer, Uint32 spawnTime, Player* player)
    : BaseEnemy(renderer), spawnTime(spawnTime), target(player), speedMultiplier(2.0f), 
      isBoosted(false), boostStartTime(0), boostDuration(1000), lastRandomMoveTime(SDL_GetTicks()) {
    // 初始化位置
    mPosX = static_cast<float>(std::rand() % SCREEN_WIDTH);
    mPosY = static_cast<float>(std::rand() % SCREEN_HEIGHT);
    mSpeed = 100.0f;  // 基礎速度
    mHP = 2000;       // 高血量
    mDamage = 50;     // 高傷害
    mRadius = 60.0f;  // 大小半徑
    mHealthBar = HealthBar(120, 5); // 將寬度改為 200
    mHealthBar.setHealth(mHP, 200);

    // 初始化隨機方向
    generateRandomDirection();

    // 加載圖片
    Texture = IMG_LoadTexture(renderer, "assets/images/Enemy_Hsieh.png");
    if (!Texture) {
        std::cerr << "Failed to load Enemy_Hsieh texture: " << IMG_GetError() << std::endl;
    }

    // 初始化血條
    mHealthBar.setHealth(mHP, 240);
}

// 析構函式
Enemy_Hsieh::~Enemy_Hsieh() {
    if (Texture) {
        SDL_DestroyTexture(Texture);
    }
}

// 更新邏輯
void Enemy_Hsieh::update(double deltaTime) {
    Uint32 currentTime = SDL_GetTicks();

    // 每 3 秒爆衝
    if (currentTime - lastRandomMoveTime > 3000) {
        isBoosted = true;
        boostStartTime = currentTime;
        generateRandomDirection(); // 更新隨機方向
        lastRandomMoveTime = currentTime;
    }

    // 如果在加速狀態，檢查加速是否結束
    if (isBoosted && currentTime - boostStartTime > boostDuration) {
        isBoosted = false;
    }

    // 移動邏輯
    float effectiveSpeed = isBoosted ? mSpeed * speedMultiplier : mSpeed;
    mPosX += randomDirX * effectiveSpeed * deltaTime;
    mPosY += randomDirY * effectiveSpeed * deltaTime;

    // 確保敵人在螢幕範圍內，並反彈
    if (mPosX < 0 || mPosX > SCREEN_WIDTH) {
    randomDirX = -randomDirX; // 反轉 X 方向
    mPosX = std::clamp(mPosX, 0.0f, static_cast<float>(SCREEN_WIDTH));
    }
    if (mPosY < 0 || mPosY > SCREEN_HEIGHT) {
    randomDirY = -randomDirY; // 反轉 Y 方向
    mPosY = std::clamp(mPosY, 0.0f, static_cast<float>(SCREEN_HEIGHT));
    }


    // 檢查與玩家的碰撞
    checkCollisionWithPlayer();
}

// 繪製邏輯
void Enemy_Hsieh::render(SDL_Renderer* renderer) {
    BaseEnemy::render(renderer);

    if (Texture) {
        SDL_Rect rect = {static_cast<int>(mPosX - mRadius), static_cast<int>(mPosY - mRadius),
                         static_cast<int>(mRadius * 2), static_cast<int>(mRadius * 2)};
        SDL_RenderCopy(renderer, Texture, NULL, &rect);
    }
}

// 生成隨機方向
void Enemy_Hsieh::generateRandomDirection() {
    do {
        randomDirX = static_cast<float>((std::rand() % 200 - 100)) / 100.0f; // 隨機範圍 [-1, 1]
        randomDirY = static_cast<float>((std::rand() % 200 - 100)) / 100.0f;

        // 規範化方向向量
        float magnitude = std::sqrt(randomDirX * randomDirX + randomDirY * randomDirY);
        if (magnitude > 0) {
            randomDirX /= magnitude;
            randomDirY /= magnitude;
        }
    } while (std::abs(randomDirX) < 0.1f && std::abs(randomDirY) < 0.1f); // 確保方向不為零
}

// 檢查與玩家的碰撞
void Enemy_Hsieh::checkCollisionWithPlayer() {
    if (!target) return;

    // 計算與玩家的距離
    float dx = target->getX() - mPosX;
    float dy = target->getY() - mPosY;
    float distance = std::sqrt(dx * dx + dy * dy);

    if (distance < mRadius + target->getRadius()) {
        // 撞擊後造成傷害
        target->takeDamage(mDamage);
        std::cout << "Enemy_Hsieh collided with the player!" << std::endl;
    }
}
