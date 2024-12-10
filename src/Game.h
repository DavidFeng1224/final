#ifndef GAME_H
#define GAME_H

#include <vector>
#include "BaseEnemy.h"
#include "Enemy_Sum.h"
#include "Enemy_Integral.h"
#include "Player.h"

class Game {
private:
    std::vector<BaseEnemy*> enemies;  // 敵人列表
    Player player;
    SDL_Texture* backgroundTexture = nullptr;
    Uint32 startTime;
    Uint32 lastSpawnTime;
    SDL_Renderer* mRenderer;

    void spawnEnemies(Uint32 elapsedTime); // 生成敵人
    void resolvePlayerEnemyCollision(BaseEnemy* enemy); // 處理玩家與敵人的碰撞

public:
    Game(SDL_Renderer* renderer);
    ~Game();

    void handleEvent(SDL_Event& e);
    void render(SDL_Renderer* renderer);
    void update(double deltaTime);
    bool checkCollision(const Bullet& bullet, const BaseEnemy& enemy) const;
    bool checkPlayerCollision(const Player& player, const BaseEnemy& enemy) const;
    bool checkEnemyCollision(const BaseEnemy& enemy1, const BaseEnemy& enemy2) const;
    void resolveEnemyOverlap(BaseEnemy* enemy1, BaseEnemy* enemy2);
};

#endif
