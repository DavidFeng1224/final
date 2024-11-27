#include "Enemy.h"
#include <cstdlib>
#include <ctime>

EnemyManager::EnemyManager(SDL_Renderer* renderer, int screenWidth, int screenHeight)
    : renderer(renderer), screenWidth(screenWidth), screenHeight(screenHeight), lastSpawnTime(0) {
    srand(static_cast<unsigned>(time(0))); // 初始化隨機種子
}

void EnemyManager::update() {
    Uint32 currentTime = SDL_GetTicks();

    // 檢查是否需要生成新敵人
    if (currentTime > lastSpawnTime + SPAWN_INTERVAL) {
        SDL_Rect enemy;
        enemy.x = rand() % (screenWidth - ENEMY_WIDTH);
        enemy.y = 0; // 從螢幕頂部生成
        enemy.w = ENEMY_WIDTH;
        enemy.h = ENEMY_HEIGHT;
        enemies.push_back(enemy);
        lastSpawnTime = currentTime;
    }

    // 更新敵人位置
    for (auto& enemy : enemies) {
        enemy.y += 5; // 每次更新向下移動 5 像素
    }

    // 移除超出螢幕的敵人
    enemies.erase(std::remove_if(enemies.begin(), enemies.end(),
                                 [this](const SDL_Rect& e) { return e.y > screenHeight; }),
                  enemies.end());
}

void EnemyManager::render() {
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255); // 紅色敵人
    for (const auto& enemy : enemies) {
        SDL_RenderFillRect(renderer, &enemy);
    }
}
