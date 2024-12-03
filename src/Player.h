#ifndef PLAYER_H
#define PLAYER_H

#include <SDL2/SDL.h>
#include <vector> // 用於存放子彈
#include "Bullet.h" // 包含 Bullet 的定義

class Player {
public:
    Player(SDL_Renderer* renderer, double speed);
    void handleEvent(SDL_Event& e);
    void update(double deltaTime);
    void render(SDL_Renderer* renderer);
    void fireBullet(int mouseX, int mouseY);

    float getX() const { return mPosX; }
    float getY() const { return mPosY; }

private:
    double mSpeed;
    double mPosX, mPosY; // 玩家當前的位置
    int mRadius;
    bool mMoveUp, mMoveDown, mMoveLeft, mMoveRight;

    std::vector<Bullet> bullets; // 用於存放子彈的容器
};

#endif
