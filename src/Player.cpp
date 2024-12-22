#include "Player.h"
#include "BaseEnemy.h"
#include <SDL2/SDL.h>

#include <algorithm>
#include <cmath>
#include <iostream>

#include "Bullet.h"
#include "Global.h"
using namespace std;

Player::Player(SDL_Renderer* renderer, double speed)
    : mSpeed(speed), mPosX(SCREEN_WIDTH / 2), mPosY(SCREEN_HEIGHT / 2), mRadius(30), mMoveUp(false), mMoveDown(false), mMoveLeft(false), mMoveRight(false), mHP(100), mHealthBar(mRadius * 2, 5)
    , bounceTimeLeft(0.0f), bounceTotalTime(0.5f),
      bounceVelocity(300.0f), bounceDirX(0.0f), bounceDirY(0.0f)
{
    if (!loadTexture(renderer, "assets/images/Player.png")) {
        std::cerr << "Failed to load player texture!" << std::endl;
    }
    mHealthBar.setHealth(mHP, 100);  // 閮剖???銵??
    mSpeed = 200.0;
    mAcc = 8.0;
    mFri = 5.0;
    mVelX = 0;
    mVelY = 0;
}

void Player::takeDamage(int damage) {
    mHP -= damage;
    if (mHP < 0) mHP = 0;
    mHealthBar.setHealth(mHP, 100);  // ?湔銵璇?
}

void Player::handleEvent(SDL_Event& e) {
    if (e.type == SDL_KEYDOWN || e.type == SDL_KEYUP) {
        bool keyState = (e.type == SDL_KEYDOWN);  // ?斗??舀?銝??舫???

        switch (e.key.keysym.sym) {
            case SDLK_UP:
            case SDLK_w:
                mMoveUp = keyState;  // 閮?UP?萇????
                break;
            case SDLK_DOWN:
            case SDLK_s:
                mMoveDown = keyState;  // 閮?DOWN?萇????
                break;
            case SDLK_LEFT:
            case SDLK_a:
                mMoveLeft = keyState;  // 閮?LEFT?萇????
                break;
            case SDLK_RIGHT:
            case SDLK_d:
                mMoveRight = keyState;  // 閮?RIGHT?萇????
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

float lerp(float a, float b, float t) {
    return a + t * (b - a);
}

float collisionCooldown = 0.2f; // 200 毫秒
float timeSinceLastCollision = 0.0f;
// 
void Player::update(double deltaTime) {
    float dirX = mMoveRight - mMoveLeft;
    float dirY = mMoveDown - mMoveUp;
    if (deltaTime > 1.0 || deltaTime <= 0.0) {
        std::cerr << "Warning: Abnormal deltaTime: " << deltaTime << std::endl;
        deltaTime = 0.016; // 使用約等於 60 FPS 的時間
    }
    if (deltaTime <= 0.0) {
        std::cerr << "Error: Invalid deltaTime value: " << deltaTime << std::endl;
        return;
    }

    if (dirX != 0 || dirY != 0) {
        double length = std::sqrt(dirX * dirX + dirY * dirY);

        dirX /= length;
        dirY /= length;

        mVelX = lerp(mVelX, mSpeed * dirX, mAcc * deltaTime);
        mVelY = lerp(mVelY, mSpeed * dirY, mAcc * deltaTime);
    } else {
        mVelX = lerp(mVelX, 0.0, mFri * deltaTime);
        mVelY = lerp(mVelY, 0.0, mFri * deltaTime);
    }

    mPosX += mVelX * deltaTime;
    mPosY += mVelY * deltaTime;

    if (bounceTimeLeft > 0.0f) {
        float progress = 1.0f - (bounceTimeLeft / bounceTotalTime); // Normalized time [0, 1]

        // 計算當前速度 (加入衰減)
        float currentBounceVelocity = bounceVelocity * (1.0f - progress);

        // 移動
        mPosX += bounceDirX * currentBounceVelocity * deltaTime;
        mPosY += bounceDirY * currentBounceVelocity * deltaTime;

        // 更新剩餘時間
        bounceTimeLeft -= deltaTime;

        // 如果時間結束，重置
        if (bounceTimeLeft <= 0.0f) {
            bounceTimeLeft = 0.0f;
        }
    }

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
            int dx = radius - w;  // Horizontal offset
            int dy = radius - h;  // Vertical offset
            if ((dx * dx + dy * dy) <= (radius * radius)) {
                SDL_RenderDrawPointF(renderer, centerX + dx, centerY + dy);
            }
        }
    }
}

void Player::startBounce(float dirX, float dirY) {
    bounceTimeLeft = bounceTotalTime; // 初始化彈開總時間

    // 計算方向單位向量
    float length = std::sqrt(dirX * dirX + dirY * dirY);
    if (length < 0.0001f) { // 避免除以零
        dirX = 1.0f;
        dirY = 0.0f;
        length = 1.0f;
    }
    bounceDirX = dirX / length;
    bounceDirY = dirY / length;

    // 確保初始速度適當
    bounceVelocity = 300.0f;
}



void Player::render(SDL_Renderer* renderer, int mouseX, int mouseY) {
    // Render bullets
    for (auto& bullet : bullets) {
        bullet.render(renderer);
    }

    // Calculate the angle (in degrees) between the player's position and the mouse
    float dirX = mouseX - mPosX;
    float dirY = mouseY - mPosY;
    float angle = atan2(dirY, dirX) * 180.0 / M_PI + 90;  // Convert radians to degrees

    // Render player as texture
    SDL_Rect renderQuad = {
        static_cast<int>(mPosX - mRadius * 1.5),
        static_cast<int>(mPosY - mRadius * 1.5),
        mRadius * 3,
        mRadius * 3};

    SDL_Point center = {(int)(mRadius * 1.5), (int)(mRadius * 1.5)};  // Center of the texture for rotation
    SDL_RenderCopyEx(renderer, mTexture, nullptr, &renderQuad, angle, &center, SDL_FLIP_NONE);

    // ?湔銵璇?蝵桐蒂皜脫?
    int healthBarX = static_cast<int>(mPosX - mRadius);
    int healthBarY = static_cast<int>(mPosY - mRadius - 10);
    mHealthBar.updatePosition(healthBarX, healthBarY);
    mHealthBar.render(renderer);
}

// 
void Player::resolveCollision(BaseEnemy& enemy, int& bounceStep) {
    float deltaX = enemy.getX() - mPosX;
    float deltaY = enemy.getY() - mPosY;
    float distance = std::sqrt(deltaX * deltaX + deltaY * deltaY);

    float overlap = mRadius + enemy.getRadius() - distance;
    if (bounceStep < 0) {
        std::cerr << "Error: bounceStep is invalid: " << bounceStep << std::endl;
        bounceStep = 0;
    }
    if (std::isnan(enemy.getX()) || std::isnan(enemy.getY())) {
        std::cerr << "Error: Invalid enemy position detected!" << std::endl;
        return;
    }
    if (overlap > 0) {
        // 1) 立即分離 (避免疊在一起)
        float dirX = deltaX / distance;
        float dirY = deltaY / distance;
        float bounceDistance = overlap;

        // 玩家馬上往 -dir 移
        mPosX -= dirX * bounceDistance;
        mPosY -= dirY * bounceDistance;

        // 敵人馬上往 +dir 移
        enemy.setPosition(
            enemy.getX() + dirX * bounceDistance,
            enemy.getY() + dirY * bounceDistance
        );

        // 2) 啟動彈開邏輯
        startBounce(-dirX, -dirY);
        enemy.startBounce(dirX, dirY);
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
        mTexture = nullptr;
    }
}