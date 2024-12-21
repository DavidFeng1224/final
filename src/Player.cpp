#include <SDL2/SDL.h>
#include <cmath>
#include "Player.h"
#include "Global.h"
#include "Bullet.h"
#include <algorithm>
#include <iostream>
using namespace std;

Player::Player(SDL_Renderer* renderer, double speed)
    : mSpeed(speed), mPosX(SCREEN_WIDTH / 2), mPosY(SCREEN_HEIGHT / 2), mRadius(30),
      mMoveUp(false), mMoveDown(false), mMoveLeft(false), mMoveRight(false), mHP(100) ,
     mHealthBar(mRadius * 2, 5) { 
    if (!loadTexture(renderer, "assets/images/Player.png")) {
        std::cerr << "Failed to load player texture!" << std::endl;
    }
    // : mSpeed(speed), mPosX(SCREEN_WIDTH / 2), mPosY(SCREEN_HEIGHT / 2), mRadius(30),
    //   mMoveUp(false), mMoveDown(false), mMoveLeft(false), mMoveRight(false), mHP(100) , mHealthBar(60, 5) { // 初始化血條
    mHealthBar.setHealth(mHP, 100); // 設定初始血量

}


void Player::takeDamage(int damage) {
    mHP -= damage;
    if (mHP < 0) mHP = 0;
    mHealthBar.setHealth(mHP, 100); // 更新血條
}

void Player::handleEvent(SDL_Event& e) {
    if (e.type == SDL_KEYDOWN || e.type == SDL_KEYUP) {
        bool keyState = (e.type == SDL_KEYDOWN);  // 判斷按鍵是按下還是鬆開

        switch (e.key.keysym.sym) {
            case SDLK_UP:
            case SDLK_w:
                mMoveUp = keyState;     // 記錄UP鍵的狀態
                break;
            case SDLK_DOWN:
            case SDLK_s:
                mMoveDown = keyState;   // 記錄DOWN鍵的狀態
                break;
            case SDLK_LEFT:
            case SDLK_a:
                mMoveLeft = keyState;   // 記錄LEFT鍵的狀態
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

void drawCircle(SDL_Renderer* renderer, int centerX, int centerY, int radius) {
    for (int w = 0; w < radius * 2; w++) {
        for (int h = 0; h < radius * 2; h++) {
            int dx = radius - w; // Horizontal offset
            int dy = radius - h; // Vertical offset
            if ((dx*dx + dy*dy) <= (radius * radius)) {
                SDL_RenderDrawPointF(renderer, centerX + dx, centerY + dy);
            }
        }
    }
}

void Player::render(SDL_Renderer* renderer, int mouseX, int mouseY) {
    // Render bullets
    for (auto& bullet : bullets) {
        bullet.render(renderer);
    }

    // Calculate the angle (in degrees) between the player's position and the mouse
    float dirX = mouseX - mPosX;
    float dirY = mouseY - mPosY;
    float angle = atan2(dirY, dirX) * 180.0 / M_PI +90;  // Convert radians to degrees

    // Render player as texture
    SDL_Rect renderQuad = {
        static_cast<int>(mPosX - mRadius * 1.5),
        static_cast<int>(mPosY - mRadius * 1.5),
        mRadius * 3,
        mRadius * 3
    };

    drawCircle(renderer, mPosX, mPosY, 5); // Draw a circle at the player's position

    SDL_Point center = {(int)(mRadius * 1.5), (int)(mRadius * 1.5)}; // Center of the texture for rotation
    SDL_RenderCopyEx(renderer, mTexture, nullptr, &renderQuad, angle, &center, SDL_FLIP_NONE);

    // 更新血條位置並渲染
    int healthBarX = static_cast<int>(mPosX - mRadius);
    int healthBarY = static_cast<int>(mPosY - mRadius - 10);
    mHealthBar.updatePosition(healthBarX, healthBarY);
    mHealthBar.render(renderer);
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
    float t = 30.0f;
    bullets.emplace_back(mPosX + dirX * t, mPosY + dirY * t, dirX, dirY, bulletSpeed);
}
bool Player::loadTexture(SDL_Renderer* renderer, const std::string& filePath) {
    SDL_Surface* surface = IMG_Load(filePath.c_str());
    if (!surface) {
        std::cerr << "Failed to load image: " << IMG_GetError() << std::endl;
        return false;
    }

    mTexture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);

    if (!mTexture) {
        std::cerr << "Failed to create texture: " << SDL_GetError() << std::endl;
        return false;
    }

    return true;
}

Player::~Player() {
    if (mTexture) {
        SDL_DestroyTexture(mTexture);
    }
}

