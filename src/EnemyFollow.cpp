#include "EnemyFollow.h"
#include <cmath>
#include <SDL2/SDL_image.h>
#include <iostream>

EnemyFollow::EnemyFollow(SDL_Renderer* renderer, Player* player)
    : BaseEnemy(renderer), target(player) {
    // 初始化敵人的位置，隨機生成非左上角的位置
    x = 200.0f; // 初始位置 X
    y = 200.0f; // 初始位置 Y

    // 加載圖片
    Texture = IMG_LoadTexture(renderer, "assets/images/EnemyFollow.png");
    if (!Texture) {
        std::cerr << "Failed to load EnemyFollow texture: " << IMG_GetError() << std::endl;
    }
}

EnemyFollow::~EnemyFollow() {
    if (Texture) {
        SDL_DestroyTexture(Texture);
    }
}

void EnemyFollow::update(double deltaTime) {
    if (!target) return; // 確保目標（玩家）存在

    // 獲取玩家的位置
    float targetX = target->getX();
    float targetY = target->getY();

    // 計算敵人到玩家的方向向量
    float dx = targetX - x;
    float dy = targetY - y;

    // 計算向量長度（距離）
    float distance = std::sqrt(dx * dx + dy * dy);

    if (distance > 0) {
        // 正規化方向向量
        dx /= distance;
        dy /= distance;

        // 每秒移動的速度（例如 100 單位/秒）
        float speed = 100.0f;

        // 根據方向向量和速度更新敵人位置
        x += dx * speed * deltaTime;
        y += dy * speed * deltaTime;
    }
}

void EnemyFollow::render(SDL_Renderer* renderer) {
    if (Texture) {
        // 渲染圖片
        SDL_Rect rect = {static_cast<int>(x), static_cast<int>(y), 50, 50}; // 調整大小（假設圖片為 50x50）
        SDL_RenderCopy(renderer, Texture, NULL, &rect);
    } else {
        // 當圖片加載失敗時，使用圓形作為後備
        int radius = 25;
        SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255); // 黃色
        for (int w = 0; w < radius * 2; w++) {
            for (int h = 0; h < radius * 2; h++) {
                int dx = radius - w; // 水平偏移
                int dy = radius - h; // 垂直偏移
                if ((dx * dx + dy * dy) <= (radius * radius)) {
                    SDL_RenderDrawPoint(renderer, static_cast<int>(x) + dx, static_cast<int>(y) + dy);
                }
            }
        }
    }
}
