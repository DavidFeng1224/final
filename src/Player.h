// 
#ifndef PLAYER_H
#define PLAYER_H

#include <SDL2/SDL.h>
#include <vector> // For storing bullets
#include "Bullet.h" // Include Bullet definition
#include "HealthBar.h"

class Player {
public:
    Player(SDL_Renderer* renderer, double speed);
    void handleEvent(SDL_Event& e);
    void update(double deltaTime);
    void render(SDL_Renderer* renderer);
    void fireBullet(int mouseX, int mouseY);
    void setPosition(float x, float y); // Declare setPosition method
    float getX() const { return mPosX; }
    float getY() const { return mPosY; }
    float getRadius() const { return mRadius; }
    void takeDamage(int damage); 
    int getHP() const { return mHP; }
    // Add this method to access the bullets
    std::vector<Bullet>& getBullets() { return bullets; } // Non-const version

private:
    double mSpeed;
    double mPosX, mPosY; // Player's current position
    int mRadius;
    bool mMoveUp, mMoveDown, mMoveLeft, mMoveRight;
    int mHP;  // 玩家生命值
    HealthBar mHealthBar; // 添加血條
    std::vector<Bullet> bullets; // Container for bullets

};

#endif
