// 

#include <iostream>
#include <vector>
#include <cmath>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>
#include "Game.h"
#include "Global.h"
#include "Enemy_Sum.h"
#include "Enemy_Integral.h"
#include "Player.h"
#include "BaseEnemy.h"

using namespace std;

// Constructor
Game::Game(SDL_Renderer* renderer)
    : player(renderer, 100) {
    // Initialize SDL_ttf
    if (TTF_Init() == -1) {
        cout << "Failed to initialize SDL_ttf: " << TTF_GetError() << endl;
        return;
    }

    // Initialize SDL_image
    if (IMG_Init(IMG_INIT_PNG) != IMG_INIT_PNG) {
        cout << "Failed to initialize SDL_image: " << IMG_GetError() << endl;
        return;
    }

    // Load background texture
    backgroundTexture = IMG_LoadTexture(renderer, "assets/images/Background_Grass.jpeg");
    if (!backgroundTexture) {
        cout << "Failed to load background texture: " << IMG_GetError() << endl;
    }

    // Initialize random enemies
    for (int i = 0; i < 3; ++i) {
        enemies.push_back(new Enemy_Sum(renderer));
    }

    // Initialize chasing enemies
    for (int i = 0; i < 2; ++i) {
        enemies.push_back(new Enemy_Integral(renderer, &player));
    }
}

// Destructor
Game::~Game() {
    // Clean up dynamically allocated enemies
    for (BaseEnemy* enemy : enemies) {
        delete enemy;
    }
    enemies.clear();

    // Destroy background texture
    if (backgroundTexture) {
        SDL_DestroyTexture(backgroundTexture);
        backgroundTexture = nullptr;
    }

    TTF_Quit();
    IMG_Quit();
}

// Handle player and game events
void Game::handleEvent(SDL_Event& e) {
    if (e.type == SDL_QUIT) {
        extern Gamemode gamemode;
        gamemode = EXIT;
        cout << "Quit event detected. Exiting game." << endl;
    }

    if (e.type == SDL_KEYDOWN) {
        if (e.key.keysym.sym == SDLK_ESCAPE) {
            extern Gamemode gamemode;
            gamemode = EXIT;
            cout << "ESC key pressed. Exiting game." << endl;
        }
    }

    player.handleEvent(e);
}

// Update game logic
void Game::update(double deltaTime) {
    // 更新玩家
    player.update(deltaTime);

    // 更新所有敵人
    for (BaseEnemy* enemy : enemies) {
        if (enemy->isAlive()) {
            enemy->update(deltaTime);
        }
    }

    // 檢查子彈與敵人的碰撞
    for (BaseEnemy* enemy : enemies) {
        for (auto& bullet : player.getBullets()) {
            if (enemy->isAlive() && bullet.isActive() &&
                checkCollision(bullet, *enemy)) {
                bullet.deactivate();
                enemy->takeDamage(50);  // 假設子彈造成 50 傷害
                applyKnockback(enemy, bullet);
            }
        }
    }

    // 檢查玩家與敵人的碰撞
    for (BaseEnemy* enemy : enemies) {
        if (enemy->isAlive() && checkPlayerCollision(player, *enemy)) {
            float dx = player.getX() - enemy->getX();
            float dy = player.getY() - enemy->getY();
            float distance = std::sqrt(dx * dx + dy * dy);

            // 避免除以零
            if (distance == 0.0f) {
                dx = 1.0f;
                dy = 0.0f;
                distance = 1.0f;
            }

            // 將方向單位化
            dx /= distance;
            dy /= distance;

            // 設定反彈距離，假設為 10 像素
            float bounceDistance = 10.0f;
            player.setPosition(
                player.getX() + dx * bounceDistance,
                player.getY() + dy * bounceDistance
            );

            // Move enemy in opposite direction
            enemy->setPosition(
                enemy->getX() - dx * bounceDistance,
                enemy->getY() - dy * bounceDistance
            );

            // 顯示碰撞事件
            cout << "Player collided with enemy and bounced!" << endl;
        

            // 假設玩家失去一定血量或遊戲結束邏輯
            player.takeDamage(20);//假設扣20滴血
            if (player.getHP() <= 0){
                cout << "Player died. Game over." << endl;
            }
            cout << "Player collided with enemy!" << endl;
            // 玩家受傷或觸發其他事件
        }


    }

    // Check collisions and resolve overlaps between enemies
    for (size_t i = 0; i < enemies.size(); ++i) {
        for (size_t j = i + 1; j < enemies.size(); ++j) {
            if (enemies[i]->isAlive() && enemies[j]->isAlive() &&
                checkEnemyCollision(*enemies[i], *enemies[j])) {
                resolveEnemyOverlap(enemies[i], enemies[j]);
            }
        }
    }

    // 移除已死亡的敵人
    enemies.erase(std::remove_if(enemies.begin(), enemies.end(),
                                 [](BaseEnemy* enemy) {
                                     if (!enemy->isAlive()) {
                                         delete enemy;  // 清除記憶體
                                         return true;
                                     }
                                     return false;
                                 }),
                  enemies.end());
}

bool Game::checkPlayerCollision(const Player& player, const BaseEnemy& enemy) const {
    float dx = player.getX() - enemy.getX();
    float dy = player.getY() - enemy.getY();
    float distanceSquared = dx * dx + dy * dy;
    float radiusSum = player.getRadius() + enemy.getRadius();
    return distanceSquared <= radiusSum * radiusSum;
}

bool Game::checkEnemyCollision(const BaseEnemy& enemy1, const BaseEnemy& enemy2) const {
    float dx = enemy1.getX() - enemy2.getX();
    float dy = enemy1.getY() - enemy2.getY();
    float distanceSquared = dx * dx + dy * dy;
    float radiusSum = enemy1.getRadius() + enemy2.getRadius();
    return distanceSquared < radiusSum * radiusSum;
}


void Game::resolveEnemyOverlap(BaseEnemy* enemy1, BaseEnemy* enemy2) {
    float dx = enemy1->getX() - enemy2->getX();
    float dy = enemy1->getY() - enemy2->getY();
    float distance = std::sqrt(dx * dx + dy * dy);

    // Avoid division by zero
    if (distance == 0.0f) {
        dx = 1.0f;
        dy = 0.0f;
        distance = 1.0f;
    }

    float radiusSum = enemy1->getRadius() + enemy2->getRadius();
    float overlap = radiusSum - distance;

    // Proportionally move the enemies apart
    float moveFactor = overlap / 2.0f;
    dx /= distance; // Normalize the direction vector
    dy /= distance;

    // Move each enemy away from the center of collision
    enemy1->setPosition(
        enemy1->getX() + dx * moveFactor,
        enemy1->getY() + dy * moveFactor
    );

    enemy2->setPosition(
        enemy2->getX() - dx * moveFactor,
        enemy2->getY() - dy * moveFactor
    );
}

void Game::applyKnockback(BaseEnemy* enemy, const Bullet& bullet) {
    float dx = enemy->getX() - bullet.getX();
    float dy = enemy->getY() - bullet.getY();
    float distance = std::sqrt(dx * dx + dy * dy);

    // Avoid division by zero
    if (distance == 0.0f) {
        dx = 1.0f;
        dy = 0.0f;
        distance = 1.0f;
    }

    // Normalize the direction vector
    dx /= distance;
    dy /= distance;

    // Set knockback distance
    float knockbackDistance = 15.0f;

    // Apply knockback to the enemy
    enemy->setPosition(
        enemy->getX() + dx * knockbackDistance,
        enemy->getY() + dy * knockbackDistance
    );
}

// Render game elements
void Game::render(SDL_Renderer* renderer) {
    SDL_SetRenderDrawColor(renderer, 204, 204, 204, 255);  // Background color
    SDL_RenderClear(renderer);

    // Render background
    if (backgroundTexture) {
        SDL_RenderCopy(renderer, backgroundTexture, NULL, NULL);
    }

    // Render player
    player.render(renderer);

    // Render all enemies
    for (BaseEnemy* enemy : enemies) {
        if (enemy->isAlive()) {
            enemy->render(renderer);
        }
    }
}

// Helper function to check collision between a bullet and an enemy
bool Game::checkCollision(const Bullet& bullet, const BaseEnemy& enemy) const {
    float dx = bullet.getX() - enemy.getX();
    float dy = bullet.getY() - enemy.getY();
    float distanceSquared = dx * dx + dy * dy;
    float radiusSum = bullet.getRadius() + enemy.getRadius();
    return distanceSquared <= radiusSum * radiusSum;
}

