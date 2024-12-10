#include "BaseEnemy.h"
#include "Global.h"
#include <iostream>
#include <algorithm>

// 構造函數
BaseEnemy::BaseEnemy(SDL_Renderer* renderer, float speed)
    : mPosX(0), mPosY(0), mSpeed(speed), mHP(100), mDamage(10), mRadius(25),
      Texture(nullptr), renderer(renderer), mHealthBar(50, 5) {
    // 提高初始速度倍率
    mSpeed *= 1.5f;

    // 初始化血條
    mHealthBar.setHealth(mHP, 100);
}

// 獲取 X 座標
float BaseEnemy::getX() const {
    return mPosX;
}

// 獲取 Y 座標
float BaseEnemy::getY() const {
    return mPosY;
}

// 獲取半徑
float BaseEnemy::getRadius() const {
    return mRadius;
}

// 設置速度
void BaseEnemy::setSpeed(float speed) {
    mSpeed = speed;
}

// 獲取速度
float BaseEnemy::getSpeed() const {
    return mSpeed;
}

// 減少敵人血量
void BaseEnemy::takeDamage(int damage) {
    mHP -= damage;
    if (mHP < 0) mHP = 0;
    mHealthBar.setHealth(mHP, 100); // 更新血條
}

// 檢查敵人是否存活
bool BaseEnemy::isAlive() const {
    return mHP > 0;
}

// 設置敵人位置
void BaseEnemy::setPosition(float x, float y) {
    mPosX = std::clamp(x, static_cast<float>(mRadius), static_cast<float>(SCREEN_WIDTH - mRadius));
    mPosY = std::clamp(y, static_cast<float>(mRadius), static_cast<float>(SCREEN_HEIGHT - mRadius));
}

// 渲染敵人及其血條
void BaseEnemy::render(SDL_Renderer* renderer) {
    if (Texture) {
        SDL_Rect rect = {static_cast<int>(mPosX - mRadius), static_cast<int>(mPosY - mRadius),
                         static_cast<int>(mRadius * 2), static_cast<int>(mRadius * 2)};
        SDL_RenderCopy(renderer, Texture, NULL, &rect);
    }

    // 渲染血條
    int healthBarX = static_cast<int>(mPosX - mRadius);
    int healthBarY = static_cast<int>(mPosY - mRadius - 10);
    mHealthBar.updatePosition(healthBarX, healthBarY);
    mHealthBar.render(renderer);
}
