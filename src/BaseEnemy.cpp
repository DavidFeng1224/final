#include "BaseEnemy.h"
#include "Global.h"
#include <iostream>
#include <algorithm>

BaseEnemy::BaseEnemy(SDL_Renderer* renderer)
    : mPosX(0), mPosY(0), mSpeed(0), mHP(100), mDamage(10), mRadius(25),
      Texture(nullptr), renderer(renderer), mHealthBar(50, 5) {
    mHealthBar.setHealth(mHP, 100); // 初始化血條
}

void BaseEnemy::takeDamage(int damage) {
    mHP -= damage;
    if (mHP < 0) mHP = 0;
    mHealthBar.setHealth(mHP, 100); // 更新血條
}

bool BaseEnemy::isAlive() const {
    return mHP > 0;
}

void BaseEnemy::setPosition(float x, float y) {
    mPosX = std::clamp(x, static_cast<float>(mRadius), static_cast<float>(SCREEN_WIDTH - mRadius));
    mPosY = std::clamp(y, static_cast<float>(mRadius), static_cast<float>(SCREEN_HEIGHT - mRadius));
}

void BaseEnemy::render(SDL_Renderer* renderer) {
    // 渲染敵人本體
    if (Texture) {
        SDL_Rect rect = {static_cast<int>(mPosX - mRadius), static_cast<int>(mPosY - mRadius),
                         static_cast<int>(mRadius * 2), static_cast<int>(mRadius * 2)};
        SDL_RenderCopy(renderer, Texture, NULL, &rect);
    }

    // 更新血條位置並渲染
    int healthBarX = static_cast<int>(mPosX - mRadius);  // 血條位於頭部上方
    int healthBarY = static_cast<int>(mPosY - mRadius - 10); // 偏移距離 10
    mHealthBar.updatePosition(healthBarX, healthBarY);
    mHealthBar.render(renderer);
}
