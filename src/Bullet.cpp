#include "Bullet.h"
#include "Global.h"
#include <cmath>
#include <iostream>

// Constructor
Bullet::Bullet(float startX, float startY, float dirX, float dirY, float speed)
    : x(startX), y(startY), size(10), active(true), texture(nullptr) {
    speedX = dirX * speed;
    speedY = dirY * speed;
}

// Destructor to free the texture
Bullet::~Bullet() {
    if (texture) {
        SDL_DestroyTexture(texture);
        texture = nullptr;
    }
}

// Load the texture for the bullet
bool Bullet::loadTexture(SDL_Renderer* renderer, const std::string& filePath) {
    SDL_Surface* surface = IMG_Load(filePath.c_str());
    if (!surface) {
        std::cerr << "Failed to load bullet image: " << IMG_GetError() << std::endl;
        return false;
    }
    texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);

    if (!texture) {
        std::cerr << "Failed to create bullet texture: " << SDL_GetError() << std::endl;
        return false;
    }
    return true;
}

// Update bullet position
void Bullet::update(double deltaTime) {
    if (!active) return;

    x += speedX * deltaTime;
    y += speedY * deltaTime;

    if (std::isnan(x) || std::isnan(y)) {
        active = false;
        return;
    }

    // Deactivate bullet if it goes out of bounds
    if (x < 0 || x > SCREEN_WIDTH || y < 0 || y > SCREEN_HEIGHT) {
        active = false;
    }
}

// Render the bullet
void Bullet::render(SDL_Renderer* renderer) {
    if (!active) return;

    if (texture) {
        SDL_Rect renderQuad = { static_cast<int>(x - size / 2), static_cast<int>(y - size / 2), size, size };
        SDL_RenderCopy(renderer, texture, nullptr, &renderQuad); // Render the bullet texture
    } else {
        // Fallback: render a circle if the texture is not loaded
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);  // black
        int radius = size / 2;
        for (int w = -radius; w <= radius; w++) {
            for (int h = -radius; h <= radius; h++) {
                if (w * w + h * h <= radius * radius) {
                    SDL_RenderDrawPoint(renderer, static_cast<int>(x) + w, static_cast<int>(y) + h);
                }
            }
        }
    }
}

// Check if the bullet is active
bool Bullet::isActive() const {
    return active;
}

// Deactivate the bullet
void Bullet::deactivate() {
    active = false;
}
