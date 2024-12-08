// 
#ifndef BASEENEMY_H
#define BASEENEMY_H

#include <SDL2/SDL.h>
#include "HealthBar.h"

class BaseEnemy {
protected:
    float mPosX, mPosY; // Position
    float mSpeed;       // Speed
    int mHP;            // Health points
    int mDamage;        // Attack damage
    float mRadius;      // Size
    SDL_Texture* Texture;
    SDL_Renderer* renderer;
    HealthBar mHealthBar; // Health bar

public:
    BaseEnemy(SDL_Renderer* renderer);
    virtual ~BaseEnemy() = default;

    virtual void update(double deltaTime) = 0;
    virtual void render(SDL_Renderer* renderer);

    // Make these methods public for external access
    float getX() const { return mPosX; }
    float getY() const { return mPosY; }
    float getRadius() const { return mRadius; }

    void takeDamage(int damage); // Reduce health
    bool isAlive() const;        // Check if alive
};

#endif

