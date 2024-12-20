#include "Enemy_Sum.h"
#include <cmath> // 提供數學運算函數，如 sqrt
#include <SDL2/SDL_image.h>
#include <iostream>

/**
 * @brief 構造函數。
 * 初始化敵人的速度、位置、材質和血條。
 */
Enemy_Sum::Enemy_Sum(SDL_Renderer *renderer)
    : BaseEnemy(renderer), mSpeedX(100.0f), mSpeedY(100.0f) {
    // 初始化隨機位置
    mPosX = static_cast<float>(std::rand() % SCREEN_WIDTH);
    mPosY = static_cast<float>(std::rand() % SCREEN_HEIGHT);

    mHP = 120;       // 設定血量
    mDamage = 10;    // 設定攻擊力
    mRadius = 25.0f; // 設定半徑大小

    // 初始化血條
    mHealthBar.setHealth(mHP, 120);

    // 加載圖片
    Texture = IMG_LoadTexture(renderer, "assets/images/Enemy_Sum.png");
    if (!Texture) {
        std::cerr << "Failed to load Enemy_Sum texture: " << IMG_GetError() << std::endl;
    }
}

/**
 * @brief 析構函數。
 * 清理材質資源。
 */
Enemy_Sum::~Enemy_Sum() {
    if (Texture) {
        SDL_DestroyTexture(Texture);
    }
}

/**
 * @brief 更新敵人的狀態，包括移動和處理與其他敵人的碰撞。
 * @param deltaTime 每一幀的時間間隔。
 * @param otherEnemies 其他敵人的列表。
 */
void Enemy_Sum::update(double deltaTime, const std::vector<BaseEnemy*>& otherEnemies) {
    // 更新位置
    mPosX += mSpeedX * deltaTime;
    mPosY += mSpeedY * deltaTime;

    // 檢查邊界碰撞
    if (mPosX - mRadius < 0 || mPosX + mRadius > SCREEN_WIDTH) {
        reflect(-1.0f, 0.0f); // 水平方向反射
    }
    if (mPosY - mRadius < 0 || mPosY + mRadius > SCREEN_HEIGHT) {
        reflect(0.0f, -1.0f); // 垂直方向反射
    }

    // 處理與其他敵人的碰撞
    for (BaseEnemy* other : otherEnemies) {
        if (other == this || !other->isAlive()) continue;

        float dx = other->getX() - mPosX;
        float dy = other->getY() - mPosY;
        float distance = std::sqrt(dx * dx + dy * dy);

        if (distance < mRadius + other->getRadius()) {
            // 計算碰撞法線
            float normX = dx / distance;
            float normY = dy / distance;

            // 反射速度
            reflect(normX, normY);

            // 更新位置，避免重疊
            float overlap = (mRadius + other->getRadius() - distance);
            mPosX -= normX * overlap / 2;
            mPosY -= normY * overlap / 2;
        }
    }
}

/**
 * @brief 更新敵人的狀態（重載函數）。
 * @param deltaTime 每一幀的時間間隔。
 */
void Enemy_Sum::update(double deltaTime) {
    update(deltaTime, std::vector<BaseEnemy*>());
}

/**
 * @brief 渲染敵人及其血條。
 * @param renderer SDL 渲染器指針，用於繪製敵人。
 */
void Enemy_Sum::render(SDL_Renderer *renderer) {
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

    // 渲染血條
    int healthBarX = static_cast<int>(mPosX - mRadius);
    int healthBarY = static_cast<int>(mPosY - mRadius - 10);
    mHealthBar.updatePosition(healthBarX, healthBarY);
    mHealthBar.render(renderer);
}

/**
 * @brief 計算反射向量。
 * @param normX 碰撞法線的 X 分量。
 * @param normY 碰撞法線的 Y 分量。
 */
void Enemy_Sum::reflect(float normX, float normY) {
    float dotProduct = mSpeedX * normX + mSpeedY * normY;
    mSpeedX -= 2 * dotProduct * normX;
    mSpeedY -= 2 * dotProduct * normY;
}
