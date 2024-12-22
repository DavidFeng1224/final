#ifndef ENEMY_ANDGATE_H
#define ENEMY_ANDGATE_H

#include <vector>

#include "BaseEnemy.h"
#include "Global.h"

class Enemy_ANDGate : public BaseEnemy {
   public:
    Enemy_ANDGate(SDL_Renderer* renderer);
    ~Enemy_ANDGate();

    void update(double deltaTime) override;
    void update(double deltaTime, const std::vector<BaseEnemy*>& otherEnemies);
    void render(SDL_Renderer* renderer) override;

   private:
    float mSpeedX;               // 水平速度
    float normalSpeed;           // 正常速度
    float speedBoostMultiplier;  // 加速倍率
    Uint32 lastSpeedBoostTime;   // 上次加速的時間戳
    bool isSpeedBoosted;         // 是否處於加速狀態
    bool mIsInMap;               // 是否在地圖內

    Uint32 speedBoostCooldown;  // 加速冷卻時間
    Uint32 speedBoostDuration;  // 加速持續時間

    void checkWallCollision();                                              // 檢測牆壁碰撞
    void checkEnemyCollision(const std::vector<BaseEnemy*>& otherEnemies);  // 檢測敵人碰撞
};

#endif
