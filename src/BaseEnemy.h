#ifndef BASEENEMY_H
#define BASEENEMY_H

#include <SDL2/SDL.h>
#include "HealthBar.h"

class BaseEnemy {
protected:
    float mPosX, mPosY;  // 位置
    float mSpeed;        // 速度
    int mHP;             // 血量
    int mDamage;         // 傷害值
    float mRadius;       // 半徑（大小）
    SDL_Texture* Texture;
    SDL_Renderer* renderer;
    HealthBar mHealthBar; // 血條

public:
    BaseEnemy(SDL_Renderer* renderer);
    virtual ~BaseEnemy() = default;

    virtual void update(double deltaTime) = 0;
    virtual void render(SDL_Renderer* renderer);

    float getX() const { return mPosX; }
    float getY() const { return mPosY; }
    float getRadius() const { return mRadius; }
    void takeDamage(int damage);  // 減少血量
    bool isAlive() const;         // 檢查是否存活
    void setPosition(float x, float y); // 設置敵人位置
};

#endif
