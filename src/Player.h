#ifndef PLAYER_H
#define PLAYER_H

#include <SDL2/SDL.h>

class Player {
public:
    Player(SDL_Renderer* renderer, double speed);  // Constructor
    void handleEvent(SDL_Event& e);  // 處理鍵盤事件
    void update(double deltaTime);  // 更新玩家位置
    void render(SDL_Renderer* renderer);  // 渲染玩家

private:
    double mSpeed;  // 玩家速度
    double mPosX, mPosY;  // 玩家位置
    int mRadius;  // 玩家半徑
    bool mMoveUp, mMoveDown, mMoveLeft, mMoveRight;  // 按鍵狀態
};

#endif // PLAYER_H
