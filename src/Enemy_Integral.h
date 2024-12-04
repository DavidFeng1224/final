#ifndef ENEMY_INTEGRAL_H
#define ENEMY_INTEGRAL_H

#include "BaseEnemy.h"
#include "Player.h" 

class Enemy_Integral : public BaseEnemy {
public:
    Enemy_Integral(SDL_Renderer *renderer, Player *player);
    ~Enemy_Integral(); // 確保 destructor 已明確宣告
    void update(double deltaTime) override;
    void render(SDL_Renderer* renderer) override; // 確保 render 已正確宣告

private:
    Player* target;
};

#endif
