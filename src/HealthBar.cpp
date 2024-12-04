#include "HealthBar.h"

HealthBar::HealthBar(int width, int height)
    : mCurrentHealth(100), mMaxHealth(100), mWidth(width), mHeight(height),
      mPosX(0), mPosY(0),
      mBackgroundColor({255, 0, 0, 255}), // 背景顏色：紅色
      mForegroundColor({0, 255, 0, 255}) // 前景顏色：綠色
{}

void HealthBar::setHealth(int current, int max) {
    mCurrentHealth = current;
    mMaxHealth = max;
}

void HealthBar::updatePosition(int x, int y) {
    mPosX = x;
    mPosY = y;
}

void HealthBar::render(SDL_Renderer* renderer) {
    // 設定背景顏色
    SDL_SetRenderDrawColor(renderer, mBackgroundColor.r, mBackgroundColor.g, mBackgroundColor.b, mBackgroundColor.a);
    SDL_Rect backgroundRect = {mPosX, mPosY, mWidth, mHeight};
    SDL_RenderFillRect(renderer, &backgroundRect);

    // 設定前景顏色（血量）
    float healthRatio = static_cast<float>(mCurrentHealth) / mMaxHealth;
    SDL_SetRenderDrawColor(renderer, mForegroundColor.r, mForegroundColor.g, mForegroundColor.b, mForegroundColor.a);
    SDL_Rect foregroundRect = {mPosX, mPosY, static_cast<int>(mWidth * healthRatio), mHeight};
    SDL_RenderFillRect(renderer, &foregroundRect);
}
