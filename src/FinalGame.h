#ifndef FINALGAME_H
#define FINALGAME_H

#include <SDL2/SDL.h>
#include <vector>
#include <queue>
#include "BaseEnemy.h"
#include "Enemy_Sum.h"
#include "Enemy_Integral.h"
#include "Enemy_ANDGate.h"
#include "Enemy_Hsieh.h"
#include "Player.h"
#include "Global.h" // 引入 Global.h 使用 Wave 定義

class FinalGame {
private:
    // 成員變數
    std::vector<BaseEnemy*> enemies; // 保存所有敵人實例的向量
    Player player;                   // 玩家實例
    SDL_Texture* backgroundTexture;  // 背景圖片紋理
    SDL_Renderer* mRenderer;         // 渲染器
    Uint32 startTime;                // 遊戲開始時間
    Uint32 lastSpawnTime;            // 上次生成敵人的時間
    float spawnDistance;             // 敵人生成的距離半徑
    bool enemyHsiehAlive;            // 判定 Enemy_Hsieh 是否存活
    bool hasSpawnedHsieh;            // 是否生成過 Enemy_Hsieh
    bool IsWin;
    bool isPositionOverlapping(float x, float y, float radius) const;

    // 私有函式
    void spawnEnemies();                 // 生成敵人
    void spawnEnemyHsieh();              // 生成特殊敵人 Enemy_Hsieh
    void spawnEnemySum();                // 生成 Sum 類型敵人
    void spawnEnemyIntegral();           // 生成 Integral 類型敵人
    void spawnEnemyANDGate();            // 生成 ANDGate 類型敵人

public:
    FinalGame(SDL_Renderer* renderer);   // 構造函式
    ~FinalGame();                        // 析構函式

    void handleEvent(SDL_Event& e);      // 處理事件
    void update(double deltaTime);       // 更新遊戲邏輯
    void render(SDL_Renderer* renderer); // 繪製所有遊戲元素

    // 碰撞檢測函式
    bool checkCollision(const Bullet& bullet, const BaseEnemy& enemy) const;
    bool checkPlayerCollision(const Player& player, const BaseEnemy& enemy) const;
    void resolvePlayerEnemyCollision(BaseEnemy* enemy);

};

#endif
