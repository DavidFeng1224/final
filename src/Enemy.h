#ifndef ENEMY_H
#define ENEMY_H

#include <SDL2/SDL.h>
#include "Global.h"

class Enemy {
public:
    // Constructor and destructor
    Enemy(SDL_Renderer* renderer);
    ~Enemy();

    // Update position
    void update(double deltaTime);

    // Render enemy on the screen
    void render(SDL_Renderer* renderer);

private:
    // Enemy position and movement
    float x, y;
    float speedX, speedY;

    // SDL texture for enemy
    SDL_Texture* Texture;
};

#endif
