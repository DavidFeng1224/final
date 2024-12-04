#ifndef BASEENEMY_H
#define BASEENEMY_H

#include <SDL2/SDL.h>
#include "HealthBar.h"

class BaseEnemy {
protected:
    float mPosX, mPosY; // 位置
    float mSpeed;       // 速度
    int mHP;            // 血量
    int mDamage;        // 攻擊力
    float mRadius;      // 大小
    SDL_Texture* Texture;
    SDL_Renderer* renderer;
    HealthBar mHealthBar; // 血條

public:
    BaseEnemy(SDL_Renderer* renderer);
    virtual ~BaseEnemy() = default;

    virtual void update(double deltaTime) = 0;
    virtual void render(SDL_Renderer* renderer);

    void takeDamage(int damage); // 扣除血量
    bool isAlive() const;        // 是否存活
};

#endif
