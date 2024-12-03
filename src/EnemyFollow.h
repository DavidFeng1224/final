#ifndef ENEMYFOLLOW_H
#define ENEMYFOLLOW_H

#include "BaseEnemy.h"
#include "Player.h" // Assuming you have a Player class

class EnemyFollow : public BaseEnemy {
public:
    EnemyFollow(SDL_Renderer* renderer, Player* player);
    void update(double deltaTime) override;

private:
    Player* target;
};

#endif
