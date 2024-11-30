#include <SDL2/SDL.h>
#include <cmath>
#include "Player.h"
#include "Global.h"
#include "Bullet.h"
#include <iostream>
using namespace std;
Player::Player(SDL_Renderer* renderer, double speed)
    : mSpeed(speed), mPosX(SCREEN_WIDTH / 2), mPosY(SCREEN_HEIGHT / 2), mRadius(30),
      mMoveUp(false), mMoveDown(false), mMoveLeft(false), mMoveRight(false) {}

void Player::handleEvent(SDL_Event& e) {
    if (e.type == SDL_KEYDOWN || e.type == SDL_KEYUP) {
        bool keyState = (e.type == SDL_KEYDOWN);  // 判斷按鍵是按下還是鬆開

        switch (e.key.keysym.sym) {
            case SDLK_UP:
                mMoveUp = keyState;  // 記錄UP鍵的狀態
                break;
            case SDLK_DOWN:
                mMoveDown = keyState;  // 記錄DOWN鍵的狀態
                break;
            case SDLK_LEFT:
                mMoveLeft = keyState;  // 記錄LEFT鍵的狀態
                break;
            case SDLK_RIGHT:
                mMoveRight = keyState;  // 記錄RIGHT鍵的狀態
                break;
            default:
                break;
        }
    }

     if (e.type == SDL_MOUSEBUTTONDOWN && e.button.button == SDL_BUTTON_LEFT) {
         int mouseX, mouseY;
         SDL_GetMouseState(&mouseX, &mouseY);
         fireBullet(mouseX, mouseY);  // Fire bullet towards the mouse position
     }
}

void Player::update(double deltaTime) {
    // 根據按鍵狀態來更新玩家的位置

    if (mMoveUp) mPosY -= mSpeed * deltaTime;
    if (mMoveDown) mPosY += mSpeed * deltaTime;
    if (mMoveLeft) mPosX -= mSpeed * deltaTime;
    if (mMoveRight) mPosX += mSpeed * deltaTime;

    

    if (mPosX - mRadius < 0) mPosX = mRadius;  // Left boundary
    if (mPosX + mRadius > SCREEN_WIDTH) mPosX = SCREEN_WIDTH - mRadius;  // Right boundary
    if (mPosY - mRadius < 0) mPosY = mRadius;  // Top boundary
    if (mPosY + mRadius > SCREEN_HEIGHT) mPosY = SCREEN_HEIGHT - mRadius;  // Bottom boundary


    for (auto& bullet : bullets) {
         bullet.update(deltaTime);
    }

     // Remove inactive bullets
     bullets.erase(std::remove_if(bullets.begin(), bullets.end(),
                                 [](const Bullet& b) { return !b.isActive(); }),
                   bullets.end());
}

void Player::render(SDL_Renderer* renderer) {
    int centerX = static_cast<int>(mPosX);
    int centerY = static_cast<int>(mPosY);
    int radius = mRadius;

    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);  // 玩家顏色設為紅色

    for (int w = -radius; w < radius; w++) {
        for (int h = -radius; h < radius; h++) {
            if (w * w + h * h <= radius * radius) {
                SDL_RenderDrawPoint(renderer, centerX + w, centerY + h);  // 繪製圓形
            }
        }

    }
    for (auto& bullet : bullets) {
        bullet.render(renderer);
    }
    
}
void Player::fireBullet(int mouseX, int mouseY) {
    // Calculate direction vector
     float dirX = mouseX - mPosX;
     float dirY = mouseY - mPosY;
     float magnitude = std::sqrt(dirX * dirX + dirY * dirY);

     // 防止方向向量長度為零
    if (magnitude < 0.001f) return;// 避免過小值
    


     // Normalize direction vector and set bullet speed
     dirX /= magnitude;
     dirY /= magnitude;
     float bulletSpeed = 300.0f;  // Example bullet speed
     // Add a new bullet to the vector
     bullets.emplace_back(mPosX, mPosY, dirX, dirY, bulletSpeed);
}
