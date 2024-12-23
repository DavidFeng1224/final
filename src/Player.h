//
#ifndef PLAYER_H
#define PLAYER_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include <string>  // for file path string
#include <vector>  // For storing bullets
#include "BaseEnemy.h"
#include "Bullet.h"  // Include Bullet definition
#include "HealthBar.h"

class Player {
   public:
    Player(SDL_Renderer* renderer, double speed);
    ~Player();  // destructor to free resources

    void handleEvent(SDL_Event& e);
    void update(double deltaTime);
    void render(SDL_Renderer* renderer, int mouseX, int mouseY);
    void resolveCollision(BaseEnemy& enemy, int& bounceStep);
    void fireBullet(int mouseX, int mouseY);
    void setPosition(float x, float y);  // Declare setPosition method
    void startBounce(float dirX, float dirY);


    float getX() const { return mPosX; }
    float getY() const { return mPosY; }
    float getRadius() const { return mRadius; }
    void takeDamage(int damage);
    int getHP() const { return mHP; }

    // Add this method to access the bullets
    std::vector<Bullet>& getBullets() { return bullets; }  // Non-const version

   private:
    double mSpeed;
    double mPosX, mPosY;  // Player's current position
    double mVelX, mVelY;  // Player's current velocity
    double mAcc;          // Player's acceleration
    double mFri;          // Player's friction
    int mRadius;
    bool mMoveUp, mMoveDown, mMoveLeft, mMoveRight;
    int mHP;                      // 玩家生命值
    float bounceDirX;         // 彈開方向 X
    float bounceDirY;         // 彈開方向 Y
    float bounceTimeLeft;     // 剩餘彈開時間 (秒)
    float bounceTotalTime;    // 總彈開時間
    float bounceVelocity;     // 初始速度 (像素/秒)
    float damping;            // 速度衰減參數

    HealthBar mHealthBar;         // 添加血條
    std::vector<Bullet> bullets;  // Container for bullets

    // Texture for player image
    SDL_Texture* mTexture;  // Add a texture for the player

    // Helper method to load texture
    bool loadTexture(SDL_Renderer* renderer, const std::string& filePath);
};

#endif
