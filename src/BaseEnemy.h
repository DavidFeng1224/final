#ifndef BASEENEMY_H
#define BASEENEMY_H

#include <SDL2/SDL.h>

class BaseEnemy {
protected:
    float x, y; // Position (protected so derived classes can access)
    SDL_Texture* Texture;
    SDL_Renderer* renderer;

public:
    BaseEnemy(SDL_Renderer* renderer)
        : x(0), y(0), Texture(nullptr), renderer(renderer) {}
    
    virtual ~BaseEnemy() = default;  // 確保是多態類別

    // 純虛函數，讓派生類實作
    virtual void update(double deltaTime) = 0;

    // 非純虛函數，提供默認行為
    virtual void render(SDL_Renderer* renderer);
};

#endif
