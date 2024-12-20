#include "Enemy_ANDGate.h"
#include <SDL2/SDL_image.h>
#include <cmath>
#include <iostream>

/**
 * @brief 構造函數。
 * 初始化敵人的位置、速度和屬性。
 */
Enemy_ANDGate::Enemy_ANDGate(SDL_Renderer* renderer)
    : BaseEnemy(renderer), mSpeedX(100.0f) {
    // 初始化隨機位置
    mPosX = static_cast<float>(std::rand() % SCREEN_WIDTH);
    mPosY = static_cast<float>(std::rand() % SCREEN_HEIGHT);

    mHP = 100;       // 設定血量
    mDamage = 15;    // 設定攻擊力
    mRadius = 20.0f; // 設定半徑大小

    // 初始化血條
    mHealthBar.setHealth(mHP, 100);

    // 加載圖片
    Texture = IMG_LoadTexture(renderer, "assets/images/Enemy_ANDGate.png");
    if (!Texture) {
        std::cerr << "Failed to load Enemy_ANDGate texture: " << IMG_GetError() << std::endl;
    }
}

/**
 * @brief 析構函數。
 */
Enemy_ANDGate::~Enemy_ANDGate() {
    if (Texture) {
        SDL_DestroyTexture(Texture); // 釋放圖片材質
    }
}

/**
 * @brief 更新敵人的狀態。
 * @param deltaTime 每一幀的時間間隔。
 */
void Enemy_ANDGate::update(double deltaTime) {
    // 更新水平位置
    mPosX += mSpeedX * deltaTime;

    // 檢查邊界碰撞，進行反彈
    if (mPosX - mRadius < 0 || mPosX + mRadius > SCREEN_WIDTH) {
        reflect();
    }

    // 這裡需要傳入其他敵人列表，進行碰撞檢測
    // 因此需要在 Game.cpp 中修改邏輯來傳遞敵人列表
}

/**
 * @brief 處理與其他敵人的水平碰撞。
 * @param otherEnemies 其他敵人的列表。
 */
void Enemy_ANDGate::handleHorizontalCollisions(const std::vector<BaseEnemy*>& otherEnemies) {
    for (BaseEnemy* other : otherEnemies) {
        if (other == this || !other->isAlive()) continue;

        float dx = other->getX() - mPosX;
        float dy = other->getY() - mPosY;
        float distance = std::sqrt(dx * dx + dy * dy);

        if (distance < mRadius + other->getRadius()) {
            // 水平速度反轉
            mSpeedX = -mSpeedX;

            // 更新水平位置，避免重疊
            float overlap = (mRadius + other->getRadius() - distance);
            mPosX -= (dx / distance) * overlap / 2;
        }
    }
}

/**
 * @brief 水平方向反彈。
 */
void Enemy_ANDGate::reflect() {
    mSpeedX = -mSpeedX; // 反轉水平速度
}

/**
 * @brief 渲染敵人及其血條。
 * @param renderer SDL 渲染器指針，用於繪製敵人。
 */
void Enemy_ANDGate::render(SDL_Renderer* renderer) {
    // 準備圖片的渲染區域
    SDL_Rect rect = {static_cast<int>(mPosX - mRadius), static_cast<int>(mPosY - mRadius),
                     static_cast<int>(mRadius * 2), static_cast<int>(mRadius * 2)};

    // 渲染圖片
    if (Texture) {
        SDL_RenderCopy(renderer, Texture, NULL, &rect);
    }

    // 渲染血條
    int healthBarX = static_cast<int>(mPosX - mRadius);
    int healthBarY = static_cast<int>(mPosY - mRadius - 10);
    mHealthBar.updatePosition(healthBarX, healthBarY);
    mHealthBar.render(renderer);
}
