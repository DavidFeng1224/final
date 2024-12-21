#ifndef ENEMY_HSIEH_H
#define ENEMY_HSIEH_H

#include "BaseEnemy.h"
#include "Player.h"
#include "Global.h" // 確保可以使用全域變數和常量
#include <vector>

class Enemy_Hsieh : public BaseEnemy {
public:
    Enemy_Hsieh(SDL_Renderer* renderer, Uint32 spawnTime, const Player* player);
    ~Enemy_Hsieh();

    void update(double deltaTime) override; // 必須實作的純虛函數
    void update(double deltaTime, const std::vector<BaseEnemy*>& otherEnemies, Uint32 currentTime);
    void render(SDL_Renderer* renderer) override;

    bool canAppear(const std::vector<BaseEnemy*>& otherEnemies, Uint32 currentTime) const;

private:
    bool isActive;    // 是否已經出現
    Uint32 spawnTime; // 出現的時間戳（例如遊戲開始 20 秒後）
    const Player* player; // 指向玩家的指標，用於計算逃跑方向
};

#endif
