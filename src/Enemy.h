#ifndef ENEMY_H
#define ENEMY_H

#include <SDL2/SDL.h>

class Enemy {
public:
    // Constructor and destructor
    Enemy(SDL_Renderer* renderer, int screen_width, int screen_height);
    ~Enemy();

    // Update position
    void update(double deltaTime);

    // Render enemy on the screen
    void render(SDL_Renderer* renderer);

private:
    // Enemy position and movement
    float x, y;
    float speedX, speedY;

    // Screen boundaries
    int screenWidth, screenHeight;

    // SDL texture for enemy
    SDL_Texture* texture;
};

#endif
