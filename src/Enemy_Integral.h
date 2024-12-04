#ifndef ENEMY_INTEGRAL_H
#define ENEMY_INTEGRAL_H

#include "BaseEnemy.h"
#include "Player.h"

class Enemy_Integral : public BaseEnemy {
public:
    Enemy_Integral(SDL_Renderer *renderer, Player *player);
    ~Enemy_Integral();

    void update(double deltaTime) override;
    void render(SDL_Renderer* renderer) override;

private:
    Player* target; // 指向玩家的指標
};

#endif
