#ifndef ENEMY_SUM_H
#define ENEMY_SUM_H

#include "BaseEnemy.h"

class Enemy_Sum : public BaseEnemy {  // 繼承 BaseEnemy
public:
    Enemy_Sum(SDL_Renderer *renderer);
    ~Enemy_Sum();

    void update(double deltaTime) override;  // 實作基類純虛函數
    void render(SDL_Renderer* renderer) override;  // 實作基類虛函數

private:
    float speedX, speedY;        // Speed
    float accelerationX, accelerationY;  // Acceleration
};

#endif
