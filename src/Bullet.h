#ifndef BULLET_H
#define BULLET_H

#include <SDL2/SDL.h>

class Bullet {
private:
    float x, y;
    float speedX, speedY;
    int size;
    bool active;
    int radius;
public:
    Bullet(float startX, float startY, float dirX, float dirY, float speed);

    void update(double deltaTime);
    void render(SDL_Renderer* renderer);
    bool isActive() const;
    void deactivate();
};

#endif
