#include "Enemy.h"
#include "Global.h"
#include <SDL2/SDL_image.h>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <cmath>

using namespace std;

// Constructor
Enemy::Enemy(SDL_Renderer* renderer)
    : BaseEnemy(renderer) {  // 正確初始化 BaseEnemy
    // 初始化敵人特有的屬性
    std::srand(static_cast<unsigned>(std::time(0)));

    x = static_cast<float>(std::rand() % SCREEN_WIDTH);
    y = static_cast<float>(std::rand() % SCREEN_HEIGHT);

    float initialSpeed = static_cast<float>((std::rand() % 100) + 50) / 100.0f;
    speedX = initialSpeed * ((std::rand() % 2 == 0) ? 1 : -1);
    speedY = initialSpeed * ((std::rand() % 2 == 0) ? 1 : -1);

    accelerationX = 0.1f * ((std::rand() % 2 == 0) ? 1 : -1);
    accelerationY = 0.1f * ((std::rand() % 2 == 0) ? 1 : -1);

    Texture = IMG_LoadTexture(renderer, "assets/images/Enemy.png");
    if (!Texture) {
        std::cout << "Failed to load texture: " << IMG_GetError() << std::endl;
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
    // Apply acceleration to speed
    speedX += accelerationX * deltaTime;
    speedY += accelerationY * deltaTime;

    float maxSpeed = 5.0f;  // Maximum allowable speed
    // Manual clamp implementation
    // Manual clamp implementation
    speedX = (speedX < -maxSpeed) ? -maxSpeed : (speedX > maxSpeed ? maxSpeed : speedX);
    speedY = (speedY < -maxSpeed) ? -maxSpeed : (speedY > maxSpeed ? maxSpeed : speedY);


    // Update position based on the new speed
    x += speedX * deltaTime * 100;
    y += speedY * deltaTime * 100;

    // Check for screen boundaries and adjust
    if (x < 0) {
        x = 0;
        speedX = -speedX;  // Reverse direction
    } else if (x > SCREEN_WIDTH - 50) {  // Assuming enemy width is 50
        x = SCREEN_WIDTH - 50;
        speedX = -speedX;
    }

    if (y < 0) {
        y = 0;
        speedY = -speedY;  // Reverse direction
    } else if (y > SCREEN_HEIGHT - 50) {  // Assuming enemy height is 50
        y = SCREEN_HEIGHT - 50;
        speedY = -speedY;
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
