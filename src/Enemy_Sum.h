#ifndef ENEMY_SUM_H
#define ENEMY_SUM_H

#include "BaseEnemy.h"

class Enemy_Sum : public BaseEnemy {
public:
    Enemy_Sum(SDL_Renderer *renderer);
    ~Enemy_Sum();

    void update(double deltaTime) override;
    void render(SDL_Renderer* renderer) override;
};

#endif
