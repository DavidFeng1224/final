#ifndef ENEMY_ANDGATE_H
#define ENEMY_ANDGATE_H

#include "BaseEnemy.h"
#include "Global.h"
#include <vector>

/**
 * @class Enemy_ANDGate
 * @brief 一種只水平移動的圓形敵人，並在碰撞時水平反彈。
 */
class Enemy_ANDGate : public BaseEnemy {
public:
    /**
     * @brief 構造函數。
     * @param renderer SDL 渲染器指針，用於渲染敵人。
     */
    Enemy_ANDGate(SDL_Renderer* renderer);

    /**
     * @brief 析構函數。
     */
    ~Enemy_ANDGate();

    /**
     * @brief 更新敵人的狀態（基類純虛函數實現）。
     * @param deltaTime 每一幀的時間間隔。
     */
    void update(double deltaTime) override;

    /**
     * @brief 渲染敵人及其血條。
     * @param renderer SDL 渲染器指針，用於繪製敵人。
     */
    void render(SDL_Renderer* renderer) override;

private:
    float mSpeedX; // 水平速度
    void reflect(); // 反彈邏輯
};

#endif
