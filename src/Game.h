#ifndef GAME_H
#define GAME_H

#include <vector>
#include "BaseEnemy.h"
#include "Enemy_Sum.h"
#include "Enemy_Integral.h"
#include "Player.h"

class Game {
private:
    std::vector<BaseEnemy*> enemies;  // Use BaseEnemy* for polymorphism
    Player player;
    SDL_Texture* backgroundTexture = nullptr;

public:
    Game(SDL_Renderer* renderer);
    ~Game();

    void handleEvent(SDL_Event& e);
    void render(SDL_Renderer* renderer);
    void update(double deltaTime);
    bool checkCollision(const Bullet& bullet, const BaseEnemy& enemy) const;
    bool checkPlayerCollision(const Player& player, const BaseEnemy& enemy) const;

};

#endif
