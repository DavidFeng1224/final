#ifndef BASEENEMY_H
#define BASEENEMY_H

#include <SDL2/SDL.h>
#include "HealthBar.h"

// 基本敵人類別
class BaseEnemy {
protected:
    float mPosX, mPosY;       // 敵人位置
    float mSpeed;             // 敵人移動速度
    int mHP;                  // 血量
    int mDamage;              // 傷害值
    float mRadius;            // 半徑（敵人大小）
    SDL_Texture* Texture;
    SDL_Renderer* renderer;
    HealthBar mHealthBar;     // 血條
    // 讓敵人也能記錄「剩餘彈開幀數」、「當前移動距離」、「方向向量」等
    int   bounceFrames     = 0;   // 還剩多少幀要做彈開
    float currentBounceDist = 0;  // 這一幀要移動的距離
    float bounceDirX       = 0;   // 碰撞時的方向單位向量 (X)
    float bounceDirY       = 0;   // 碰撞時的方向單位向量 (Y)

public:
    BaseEnemy(SDL_Renderer* renderer, float speed = 1.0f); // 構造函數
    virtual ~BaseEnemy() = default;

    virtual void update(double deltaTime) = 0;   // 更新邏輯（純虛函數）
    virtual void render(SDL_Renderer* renderer); // 渲染敵人

    float getX() const;                    // 獲取 X 座標
    float getY() const;                    // 獲取 Y 座標
    float getRadius() const;               // 獲取敵人半徑
    float getSpeed() const;                // 獲取速度
    void setSpeed(float speed);            // 設置速度
    void takeDamage(int damage);           // 減少血量
    bool isAlive() const;                  // 檢查敵人是否存活
    void setPosition(float x, float y);    // 設置敵人位置
    void startBounce(float dirX, float dirY);

};

#endif
