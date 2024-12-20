#ifndef ENEMY_SUM_H
#define ENEMY_SUM_H

#include "BaseEnemy.h"
#include "Global.h"
#include <vector> // 用於存儲其他敵人列表

/**
 * @class Enemy_Sum
 * @brief 表示遊戲中會反射的敵人，並且包含血條顯示功能。
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
     * @param otherEnemies 其他敵人的列表。
     */
    void update(double deltaTime, const std::vector<BaseEnemy*>& otherEnemies);

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
    float mSpeedY; // 垂直速度

    /**
     * @brief 計算反射向量。
     * @param normX 碰撞法線的 X 分量。
     * @param normY 碰撞法線的 Y 分量。
     */
    void reflect(float normX, float normY);
};

#endif
