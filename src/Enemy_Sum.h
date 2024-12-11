#ifndef ENEMY_SUM_H
#define ENEMY_SUM_H

#include "BaseEnemy.h"
#include <vector> // 用於存儲其他敵人列表

/**
 * @class Enemy_Sum
 * @brief 表示遊戲中會反彈的敵人。
 */
class Enemy_Sum : public BaseEnemy {
public:
    /**
     * @brief 構造函數。
     * @param renderer SDL 渲染器指針，用於渲染敵人。
     */
    Enemy_Sum(SDL_Renderer *renderer);

    /**
     * @brief 析構函數。
     */
    ~Enemy_Sum();

    /**
     * @brief 更新敵人的狀態（基類純虛函數實現）。
     * @param deltaTime 每一幀的時間間隔。
     */
    void update(double deltaTime) override;

    /**
     * @brief 更新敵人的狀態，包括移動和處理與其他敵人的碰撞。
     * @param deltaTime 每一幀的時間間隔。
     * @param otherEnemies 其他敵人的列表。
     */
    void update(double deltaTime, const std::vector<BaseEnemy*>& otherEnemies);

    /**
     * @brief 渲染敵人。
     * @param renderer SDL 渲染器指針，用於繪製敵人。
     */
    void render(SDL_Renderer* renderer) override;
};

#endif
