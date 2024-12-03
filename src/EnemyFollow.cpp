#include "EnemyFollow.h"
#include <cmath>

EnemyFollow::EnemyFollow(SDL_Renderer* renderer, Player* player)
    : BaseEnemy(renderer), target(player) {
    x = 100.0f; // Initial position
    y = 100.0f;
}

void EnemyFollow::update(double deltaTime) {
    if (!target) return;

    // Get player position
    float targetX = target->getX();
    float targetY = target->getY();



    // Calculate direction
    float dx = targetX - x;
    float dy = targetY - y;
    float length = sqrt(dx * dx + dy * dy);

    if (length > 0) {
        dx /= length; // Normalize
        dy /= length;
        x += dx * 100 * deltaTime; // Adjust speed as needed
        y += dy * 100 * deltaTime;
    }
}
