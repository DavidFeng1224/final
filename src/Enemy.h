#ifndef ENEMY_H
#define ENEMY_H

#include <SDL2/SDL.h>
#include <vector>

class EnemyManager {
private:
    SDL_Renderer* renderer;
    int screenWidth;
    int screenHeight;
    std::vector<SDL_Rect> enemies;
    Uint32 lastSpawnTime;
    const Uint32 SPAWN_INTERVAL = 1000; // 每秒生成一個敵人
    const int ENEMY_WIDTH = 50;
    const int ENEMY_HEIGHT = 50;

public:
    EnemyManager(SDL_Renderer* renderer, int screenWidth, int screenHeight);
    void update();
    void render();
};

#endif
