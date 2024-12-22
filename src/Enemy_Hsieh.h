#ifndef ENEMY_HSIEH_H
#define ENEMY_HSIEH_H

#include "BaseEnemy.h"
#include "Player.h"
#include "Global.h"

class Enemy_Hsieh : public BaseEnemy {
public:
    Enemy_Hsieh(SDL_Renderer* renderer, Uint32 spawnTime, Player* player);
    ~Enemy_Hsieh();

    void update(double deltaTime) override;
    void render(SDL_Renderer* renderer) override;

private:
    Player* target;       // 指向玩家的指標
    Uint32 spawnTime;     // 生成的時間戳
    float speedMultiplier; // 加速倍率
    bool isBoosted;       // 是否正在加速狀態
    Uint32 boostStartTime;// 加速的開始時間
    Uint32 boostDuration; // 加速持續時間

    void checkCollisionWithPlayer();
};

#endif
