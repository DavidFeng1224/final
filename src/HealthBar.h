#ifndef HEALTHBAR_H
#define HEALTHBAR_H

#include <SDL2/SDL.h>

class HealthBar {
private:
    int mCurrentHealth; // 當前血量
    int mMaxHealth;     // 最大血量
    int mWidth;         // 血條寬度
    int mHeight;        // 血條高度
    int mPosX;          // 血條位置 X
    int mPosY;          // 血條位置 Y

    SDL_Color mBackgroundColor; // 血條背景顏色
    SDL_Color mForegroundColor; // 血條前景顏色

public:
    HealthBar(int width = 50, int height = 5); // 建構子
    void setHealth(int current, int max = -1);      // 設定血量
    void updatePosition(int x, int y);         // 更新位置
    void render(SDL_Renderer* renderer);       // 渲染血條

    void setSize(int width, int height);
};

#endif
