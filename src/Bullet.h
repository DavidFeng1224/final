
// 
#ifndef BULLET_H
#define BULLET_H

#include <SDL2/SDL.h>

class Bullet {
private:
    float x, y;        // Position
    float speedX, speedY; // Speed components
    int size;          // Size (radius)
    bool active;       // Active state

public:
    Bullet(float startX, float startY, float dirX, float dirY, float speed);

    // Public accessors for position and radius
    float getX() const { return x; }
    float getY() const { return y; }
    float getRadius() const { return size / 2; }

    void update(double deltaTime);
    void render(SDL_Renderer* renderer);
    bool isActive() const;
    void deactivate();
};

#endif



