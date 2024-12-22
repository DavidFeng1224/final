#include "Enemy_ANDGate.h"

#include <SDL2/SDL_image.h>

#include <cmath>
#include <iostream>

Enemy_ANDGate::Enemy_ANDGate(SDL_Renderer* renderer)
    : BaseEnemy(renderer), mSpeedX(100.0f), normalSpeed(100.0f), speedBoostMultiplier(2.0f), lastSpeedBoostTime(0), isSpeedBoosted(false) {
    mPosX = static_cast<float>(std::rand() % SCREEN_WIDTH);
    mPosY = static_cast<float>(std::rand() % SCREEN_HEIGHT);

    mHP = 100;
    mDamage = 15;
    mRadius = 20.0f;
    mIsInMap = false;

    mHealthBar.setHealth(mHP, 100);

    speedBoostCooldown = rand() % 4000 + 2000;
    speedBoostDuration = rand() % 500 + 1500;

    Texture = IMG_LoadTexture(renderer, "assets/images/Enemy_ANDGate.png");
    if (!Texture) {
        std::cerr << "Failed to load Enemy_ANDGate texture: " << IMG_GetError() << std::endl;
    }
}

void Enemy_ANDGate::update(double deltaTime, const std::vector<BaseEnemy*>& otherEnemies) {
    Uint32 currentTime = SDL_GetTicks();

    if (mIsInMap == false) {
        float dx = SCREEN_WIDTH / 2 - mPosX;
        dx = (dx > 0 ? 1 : -1);
        mPosX += dx * mSpeedX * deltaTime;

        if (mPosX > mRadius && mPosX < SCREEN_WIDTH - mRadius &&
            mPosY > mRadius && mPosY < SCREEN_HEIGHT - mRadius) {
            mIsInMap = true;
        }
    } else {
        // 加速邏輯
        if (!isSpeedBoosted && currentTime - lastSpeedBoostTime >= speedBoostCooldown) {
            speedBoostCooldown = rand() % 2000 + 4000;
            isSpeedBoosted = true;
            mSpeedX *= speedBoostMultiplier;
            lastSpeedBoostTime = currentTime;
        }
        if (isSpeedBoosted && currentTime - lastSpeedBoostTime >= speedBoostDuration) {
            speedBoostDuration = rand() % 500 + 1500;
            isSpeedBoosted = false;
            mSpeedX = (mSpeedX > 0 ? normalSpeed : -normalSpeed);
        }

        // 更新水平位置
        mPosX += mSpeedX * deltaTime;

        // 檢查牆壁碰撞
        checkWallCollision();
    }

    // 檢查敵人碰撞
    checkEnemyCollision(otherEnemies);
}

void Enemy_ANDGate::update(double deltaTime) {
    update(deltaTime, std::vector<BaseEnemy*>());
}

void Enemy_ANDGate::checkWallCollision() {
    if (mPosX - mRadius < 0 || mPosX + mRadius > SCREEN_WIDTH) {
        mSpeedX = -mSpeedX;  // 水平方向反轉
    }
}

void Enemy_ANDGate::checkEnemyCollision(const std::vector<BaseEnemy*>& otherEnemies) {
    for (const auto& enemy : otherEnemies) {
        if (enemy == this || !enemy->isAlive()) continue;

        float dx = enemy->getX() - mPosX;
        float dy = enemy->getY() - mPosY;
        float distance = std::sqrt(dx * dx + dy * dy);

        if (distance < mRadius + enemy->getRadius()) {
            mSpeedX = -mSpeedX;  // 水平方向反轉

            // 調整位置以避免重疊
            float overlap = (mRadius + enemy->getRadius() - distance);
            mPosX -= (dx / distance) * overlap / 2;
        }
    }
}

void Enemy_ANDGate::render(SDL_Renderer* renderer) {
    SDL_Rect rect = {static_cast<int>(mPosX - mRadius), static_cast<int>(mPosY - mRadius),
                     static_cast<int>(mRadius * 2), static_cast<int>(mRadius * 2)};
    if (Texture) {
        SDL_RenderCopy(renderer, Texture, NULL, &rect);
    } else {
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
        for (int w = -mRadius; w <= mRadius; w++) {
            for (int h = -mRadius; h <= mRadius; h++) {
                if (w * w + h * h <= mRadius * mRadius) {
                    SDL_RenderDrawPoint(renderer, static_cast<int>(mPosX) + w, static_cast<int>(mPosY) + h);
                }
            }
        }
    }

    int healthBarX = static_cast<int>(mPosX - mRadius);
    int healthBarY = static_cast<int>(mPosY - mRadius - 10);
    mHealthBar.updatePosition(healthBarX, healthBarY);
    mHealthBar.render(renderer);
}

Enemy_ANDGate::~Enemy_ANDGate() {
    if (Texture) {
        SDL_DestroyTexture(Texture);
    }
}
