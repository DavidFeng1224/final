#include "Enemy.h"
#include <SDL2/SDL_image.h>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <cmath>

// Constructor
Enemy::Enemy(SDL_Renderer* renderer, int screen_width, int screen_height)
    : screenWidth(screen_width), screenHeight(screen_height) {
    // Initialize random seed
    std::srand(static_cast<unsigned>(std::time(0)));

    // Randomly position the enemy within the screen boundaries
    x = static_cast<float>(std::rand() % screen_width);
    y = static_cast<float>(std::rand() % screen_height);

    // Randomize speed in both directions
    speedX = static_cast<float>((std::rand() % 200) - 100) / 100.0f;
    speedY = static_cast<float>((std::rand() % 200) - 100) / 100.0f;

    // Specify the absolute path to the enemy image
    const char* imagePath = "C:/Users/User/Desktop/final/assets/images/enemy.png";

    // Try to load enemy texture
    SDL_Surface* tempSurface = IMG_Load(imagePath);
    if (!tempSurface) {
        std::cerr << "Failed to load enemy image. Error: " << IMG_GetError() << std::endl;
        texture = nullptr;  // No texture loaded, fallback to a drawn circle
    } else {
        texture = SDL_CreateTextureFromSurface(renderer, tempSurface);
        SDL_FreeSurface(tempSurface);
    }
}

// Destructor
Enemy::~Enemy() {
    if (texture) {
        SDL_DestroyTexture(texture);
    }
}

// Update enemy position
void Enemy::update(double deltaTime) {
    x += speedX * deltaTime * 100;
    y += speedY * deltaTime * 100;

    // Check for screen boundaries and reverse direction if needed
    if (x < 0 || x > screenWidth) {
        speedX = -speedX;
        x = x < 0 ? 0 : screenWidth;
    }

    if (y < 0 || y > screenHeight) {
        speedY = -speedY;
        y = y < 0 ? 0 : screenHeight;
    }
}

// Render the enemy
void Enemy::render(SDL_Renderer* renderer) {
    if (texture) {
        // Render enemy texture
        SDL_Rect renderQuad = { static_cast<int>(x), static_cast<int>(y), 50, 50 };  // Fixed enemy size
        SDL_RenderCopy(renderer, texture, nullptr, &renderQuad);
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
