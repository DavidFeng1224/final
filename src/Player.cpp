#include <SDL2/SDL.h>
#include <cmath>
#include "Player.h"
#include "Global.h"
#include "Bullet.h"
#include <iostream>
using namespace std;

Player::Player(SDL_Renderer* renderer, double speed)
    : mSpeed(speed), mPosX(SCREEN_WIDTH / 2), mPosY(SCREEN_HEIGHT / 2), mRadius(30),
      mMoveUp(false), mMoveDown(false), mMoveLeft(false), mMoveRight(false), mHP(100) {}

void Player::handleEvent(SDL_Event& e) {
    if (e.type == SDL_KEYDOWN || e.type == SDL_KEYUP) {
        bool keyState = (e.type == SDL_KEYDOWN);  // 判斷按鍵是按下還是鬆開

        switch (e.key.keysym.sym) {
            case SDLK_UP:
            case SDLK_w:
                mMoveUp = keyState;  // 記錄UP鍵的狀態
                break;
            case SDLK_DOWN:
            case SDLK_s:
                mMoveDown = keyState;  // 記錄DOWN鍵的狀態
                break;
            case SDLK_LEFT:
            case SDLK_a:
                mMoveLeft = keyState;  // 記錄LEFT鍵的狀態
                break;
            case SDLK_RIGHT:
            case SDLK_d:
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
void Player::setPosition(float x, float y) {
    // mPosX = x;
    // mPosY = y;
    mPosX = std::clamp(x, static_cast<float>(mRadius), static_cast<float>(SCREEN_WIDTH - mRadius));
    mPosY = std::clamp(y, static_cast<float>(mRadius), static_cast<float>(SCREEN_HEIGHT - mRadius));
}

void Player::update(double deltaTime) {
    // Update player's position
    if (mMoveUp) mPosY -= mSpeed * deltaTime;
    if (mMoveDown) mPosY += mSpeed * deltaTime;
    if (mMoveLeft) mPosX -= mSpeed * deltaTime;
    if (mMoveRight) mPosX += mSpeed * deltaTime;

    // Constrain player within screen bounds
    if (mPosX - mRadius < 0) mPosX = mRadius;
    if (mPosX + mRadius > SCREEN_WIDTH) mPosX = SCREEN_WIDTH - mRadius;
    if (mPosY - mRadius < 0) mPosY = mRadius;
    if (mPosY + mRadius > SCREEN_HEIGHT) mPosY = SCREEN_HEIGHT - mRadius;

    // Update bullets
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

    // Render bullets
    for (auto& bullet : bullets) {
        bullet.render(renderer);
    }

    // Render player as a circle
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);  // Player color: red
    for (int w = -radius; w < radius; w++) {
        for (int h = -radius; h < radius; h++) {
            if (w * w + h * h <= radius * radius) {
                SDL_RenderDrawPoint(renderer, centerX + w, centerY + h);
            }
        }
    }
}

void Player::fireBullet(int mouseX, int mouseY) {
    // Calculate direction vector
    float dirX = mouseX - mPosX;
    float dirY = mouseY - mPosY;
    float magnitude = std::sqrt(dirX * dirX + dirY * dirY);

    // Prevent zero-length direction vector
    if (magnitude < 0.001f) return;

    // Normalize direction and set bullet speed
    dirX /= magnitude;
    dirY /= magnitude;
    float bulletSpeed = 300.0f;  // Example bullet speed

    // Add a new bullet to the vector
    bullets.emplace_back(mPosX, mPosY, dirX, dirY, bulletSpeed);
}
