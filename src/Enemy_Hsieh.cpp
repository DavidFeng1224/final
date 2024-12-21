#include "Enemy_Hsieh.h"
#include <SDL2/SDL_image.h>
#include <iostream>
#include <cmath>
#include <algorithm>

// Constructor
Enemy_Hsieh::Enemy_Hsieh(SDL_Renderer* renderer, Uint32 spawnTime, const Player* player)
    : BaseEnemy(renderer), isActive(false), spawnTime(spawnTime), player(player) {
    mPosX = 400.0f;  // 預設位置
    mPosY = 300.0f;  // 預設位置
    mSpeed = 150.0f; // 移動速度
    mHP = 200;       // 血量
    mDamage = 50;    // 攻擊力
    mRadius = 35.0f; // 大小半徑

    // 加載圖片資源
    Texture = IMG_LoadTexture(renderer, "assets/images/Enemy_Hsieh.png");
    if (!Texture) {
        std::cerr << "Failed to load texture for Enemy_Hsieh: " << IMG_GetError() << std::endl;
    } else {
        std::cout << "Texture loaded successfully for Enemy_Hsieh!" << std::endl;
    }
}

// Destructor
Enemy_Hsieh::~Enemy_Hsieh() {
    if (Texture) {
        SDL_DestroyTexture(Texture);
    }
}

// 判斷是否可以出現
bool Enemy_Hsieh::canAppear(const std::vector<BaseEnemy*>& otherEnemies, Uint32 currentTime) const {
    // 確保遊戲已經運行超過指定時間，且場上沒有其他敵人存活
    return (currentTime >= spawnTime) &&
           std::all_of(otherEnemies.begin(), otherEnemies.end(), [](BaseEnemy* enemy) {
               return !enemy->isAlive();
           });
}

// 更新邏輯：敵人移動行為（純虛函數實作）
void Enemy_Hsieh::update(double deltaTime) {
    if (isActive && player) {
        // 計算玩家的方向
        float dx = player->getX() - mPosX;
        float dy = player->getY() - mPosY;
        float distance = std::sqrt(dx * dx + dy * dy);

        // 避免除以零，計算反方向向量
        if (distance > 0) {
            dx /= distance;
            dy /= distance;

            // 反方向移動
            mPosX -= dx * mSpeed * deltaTime;
            mPosY -= dy * mSpeed * deltaTime;

            // 確保不超出螢幕邊界
            if (mPosX - mRadius < 0) mPosX = mRadius;
            if (mPosX + mRadius > SCREEN_WIDTH) mPosX = SCREEN_WIDTH - mRadius;
            if (mPosY - mRadius < 0) mPosY = mRadius;
            if (mPosY + mRadius > SCREEN_HEIGHT) mPosY = SCREEN_HEIGHT - mRadius;
        }
    }
}

// 更新邏輯：檢查生成條件
void Enemy_Hsieh::update(double deltaTime, const std::vector<BaseEnemy*>& otherEnemies, Uint32 currentTime) {
    if (!isActive && canAppear(otherEnemies, currentTime)) {
        isActive = true;
        std::cout << "Enemy_Hsieh has appeared!" << std::endl;
    }

    if (isActive) {
        this->update(deltaTime); // 呼叫基本移動邏輯
    }
}

// 渲染敵人
void Enemy_Hsieh::render(SDL_Renderer* renderer) {
    if (isActive) {
        SDL_Rect rect = {static_cast<int>(mPosX - mRadius), static_cast<int>(mPosY - mRadius),
                         static_cast<int>(mRadius * 2), static_cast<int>(mRadius * 2)};
        if (Texture) {
            SDL_RenderCopy(renderer, Texture, NULL, &rect); // 渲染圖片
        } else {
            SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255); // 替代方案：綠色圓形
            for (int w = -mRadius; w <= mRadius; w++) {
                for (int h = -mRadius; h <= mRadius; h++) {
                    if (w * w + h * h <= mRadius * mRadius) {
                        SDL_RenderDrawPoint(renderer, static_cast<int>(mPosX) + w, static_cast<int>(mPosY) + h);
                    }
                }
            }
        }
    }
}
