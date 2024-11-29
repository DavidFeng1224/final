#include <SDL2/SDL.h>
#include <cmath>
#include "Player.h"
#include "Global.h"

Player::Player(SDL_Renderer* renderer, double speed)
    : mSpeed(speed), mPosX(SCREEN_WIDTH / 2), mPosY(SCREEN_HEIGHT / 2), mRadius(20),
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
}

void Player::update(double deltaTime) {
    // 根據按鍵狀態來更新玩家的位置
    if (mMoveUp) mPosY -= mSpeed * deltaTime;  // 按上鍵時移動
    if (mMoveDown) mPosY += mSpeed * deltaTime;  // 按下鍵時移動
    if (mMoveLeft) mPosX -= mSpeed * deltaTime;  // 按左鍵時移動
    if (mMoveRight) mPosX += mSpeed * deltaTime;  // 按右鍵時移動
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
}
