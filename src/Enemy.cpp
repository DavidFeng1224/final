#include "Enemy.h"
#include "Global.h"
#include <SDL2/SDL_image.h>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <cmath>

using namespace std;

// Constructor
Enemy::Enemy(SDL_Renderer* renderer){
    // Initialize random seed
    std::srand(static_cast<unsigned>(std::time(0)));

    // Randomly position the enemy within the screen boundaries
    x = static_cast<float>(std::rand() % SCREEN_WIDTH);
    y = static_cast<float>(std::rand() % SCREEN_HEIGHT);

    // Randomize speed in both directions
    speedX = static_cast<float>((std::rand() % 200) - 100) / 100.0f;
    speedY = static_cast<float>((std::rand() % 200) - 100) / 100.0f;

    if (IMG_Init(IMG_INIT_PNG) != IMG_INIT_PNG) {
        cout << "Failed to initialize SDL_image: " << IMG_GetError() << endl;
        return;
    }

    Texture = IMG_LoadTexture(renderer, "assets/images/Enemy.png");  
    if (!Texture) {
        cout << "Failed to load background texture: " << IMG_GetError() << endl;
    }
}

// Destructor
Enemy::~Enemy() {
    if (Texture) {
        SDL_DestroyTexture(Texture);
    }
    IMG_Quit();
}

// Update enemy position
void Enemy::update(double deltaTime) {
    x += speedX * deltaTime * 100;
    y += speedY * deltaTime * 100;

    // Check for screen boundaries and reverse direction if needed
    if (x < 0 || x > SCREEN_WIDTH) {
        speedX = -speedX;
        x = x < 0 ? 0 : SCREEN_WIDTH;
    }

    if (y < 0 || y > SCREEN_HEIGHT) {
        speedY = -speedY;
        y = y < 0 ? 0 : SCREEN_HEIGHT;
    }
}

// Render the enemy
void Enemy::render(SDL_Renderer* renderer) {
    if (Texture) {
        // Render enemy texture
        SDL_Rect Rect = {static_cast<int>(x), static_cast<int>(y), 50, 50};
        SDL_RenderCopy(renderer, Texture, NULL, &Rect);
    } else {
        // Render a yellow circle as fallback
        int radius = 25;
        SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);  // Yellow color

        for (int w = 0; w < radius * 2; w++) {
            for (int h = 0; h < radius * 2; h++) {
                int dx = radius - w;  // horizontal offset
                int dy = radius - h;  // vertical offset
                if ((dx * dx + dy * dy) <= (radius * radius)) {
                    SDL_RenderDrawPoint(renderer, static_cast<int>(x) + dx, static_cast<int>(y) + dy);
                }
            }
        }
    }
}
