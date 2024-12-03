#ifndef ENEMYFOLLOW_H
#define ENEMYFOLLOW_H

#include "BaseEnemy.h"
#include "Player.h" 

class EnemyFollow : public BaseEnemy {
public:
    EnemyFollow(SDL_Renderer* renderer, Player* player);
    ~EnemyFollow(); // 確保 destructor 已明確宣告
    void update(double deltaTime) override;
    void render(SDL_Renderer* renderer) override; // 確保 render 已正確宣告

private:
    Player* target;
};

#endif
