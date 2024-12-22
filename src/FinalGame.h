#ifndef FINALGAME_H
#define FINALGAME_H

#include <SDL2/SDL.h>
#include <vector>
#include <queue>
#include "BaseEnemy.h"
#include "Enemy_Sum.h"
#include "Enemy_Integral.h"
#include "Enemy_ANDGate.h"
#include "Player.h"
#include "Enemy_Hsieh.h"



class FinalGame {
private:
    std::vector<BaseEnemy*> enemies;  // 敵人列表
    Player player;
    SDL_Texture* backgroundTexture = nullptr;
    Uint32 startTime;
    Uint32 lastSpawnTime;
    SDL_Renderer* mRenderer;
    float spawnDistance = 600.0f;
    int enemyLeft = 46;
    bool hasSpawnedHsieh; // 是否已生成 Enemy_Hsieh

    std::queue<Wave> waves;
    Wave currentWave;

    void spawnEnemies(); // 生成敵人
    void spawnEnemyHsieh(); // 生成 Enemy_Hsieh
    void resolvePlayerEnemyCollision(BaseEnemy* enemy); // 處理玩家與敵人的碰撞
    void spawnEnemySum();        // 生成 Enemy_Sum
    void spawnEnemyIntegral();   // 生成 Enemy_Integral
    void spawnEnemyANDGate();    // 生成 Enemy_ANDGate


public:
    FinalGame(SDL_Renderer* renderer);
    ~FinalGame();

    void handleEvent(SDL_Event& e);
    void render(SDL_Renderer* renderer);
    void update(double deltaTime);
    bool checkCollision(const Bullet& bullet, const BaseEnemy& enemy) const;
    bool checkPlayerCollision(const Player& player, const BaseEnemy& enemy) const;
};

#endif
