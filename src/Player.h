// #ifndef PLAYER_H
// #define PLAYER_H

// #include <SDL2/SDL.h>
// #include <vector>
// #include "Bullet.h"

// class Player {
// public:
//     Player(SDL_Renderer* renderer, double speed);  // Constructor
//     void handleEvent(SDL_Event& e);  // 處理鍵盤事件
//     void update(double deltaTime);  // 更新玩家位置
//     void render(SDL_Renderer* renderer);  // 渲染玩家

//     std::vector<Bullet> bullets;           // 存放所有玩家子彈的容器
//     void fireBullet(int mouseX, int mouseY); // 發射子彈函數

// private:
//     double mSpeed;  // 玩家速度
//     double mMoveX, mMoveY;
//     double mPosX, mPosY;  // 玩家位置
//     int mRadius;  // 玩家半徑
//     bool mMoveUp, mMoveDown, mMoveLeft, mMoveRight;  // 按鍵狀態
// };

// #endif 

#ifndef PLAYER_H
#define PLAYER_H

#include <SDL2/SDL.h>
#include <vector>
#include "Bullet.h"

class Player {
public:
    Player(SDL_Renderer* renderer, double speed);
    void handleEvent(SDL_Event& e);
    void update(double deltaTime);
    void render(SDL_Renderer* renderer);
    void fireBullet(int mouseX, int mouseY);

    std::vector<Bullet> bullets;

private:
    double mSpeed;
    double mVelX, mVelY; // Track player's current velocity
    double mPosX, mPosY;
    int mRadius;
    bool mMoveUp, mMoveDown, mMoveLeft, mMoveRight;
};

#endif
