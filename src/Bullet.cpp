#include "Bullet.h"
#include "Global.h"
#include <cmath>

// Constructor
Bullet::Bullet(float startX, float startY, float dirX, float dirY, float speed)
    : x(startX), y(startY), size(10), active(true) {
    speedX = dirX * speed;
    speedY = dirY * speed;
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

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);  // black
    // Draw the circle using midpoint circle algorithm or point-based approach
    int radius = size / 2;
    for (int w = -radius; w <= radius; w++) {
        for (int h = -radius; h <= radius; h++) {
            if (w * w + h * h <= radius * radius) {
                SDL_RenderDrawPoint(renderer, static_cast<int>(x) + w, static_cast<int>(y) + h);
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
