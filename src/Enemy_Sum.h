#ifndef ENEMY_SUM_H
#define ENEMY_SUM_H

#include "BaseEnemy.h"
#include "Global.h"
#include <vector> 

class Enemy_Sum : public BaseEnemy {
public:
    Enemy_Sum(SDL_Renderer *renderer);
    ~Enemy_Sum();

    void update(double deltaTime, const std::vector<BaseEnemy*>& otherEnemies);
    void update(double deltaTime) override;
    void render(SDL_Renderer* renderer) override;

private:
    float mSpeedX;
    float mSpeedY;
    bool mIsInMap;
    void reflect(float normX, float normY);
};

#endif
