#ifndef ENEMY_H
#define ENEMY_H

#include "BaseEnemy.h"

class Enemy : public BaseEnemy {  // 繼承 BaseEnemy
public:
    Enemy(SDL_Renderer* renderer);
    ~Enemy();

    void update(double deltaTime) override;  // 實作基類純虛函數
    void render(SDL_Renderer* renderer) override;  // 實作基類虛函數

private:
    float speedX, speedY;        // Speed
    float accelerationX, accelerationY;  // Acceleration
};

#endif
