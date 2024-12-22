#include "FinalGame.h"

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_ttf.h>

#include <algorithm>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <vector>

#include "AudioManager.h"
#include "BaseEnemy.h"
#include "Enemy_ANDGate.h"
#include "Enemy_Integral.h"
#include "Enemy_Sum.h"
#include "Enemy_Hsieh.h" // Include Enemy_Hsieh class
#include "Global.h"
#include "Player.h"

using namespace std;

// Constructor
FinalGame::FinalGame(SDL_Renderer* renderer)
    : player(renderer, 100), mRenderer(renderer), hasSpawnedHsieh(false) {
    // Initialize SDL_ttf
    if (TTF_Init() == -1) {
        cout << "Failed to initialize SDL_ttf: " << TTF_GetError() << endl;
        return;
    }

    // Initialize SDL_image for loading images
    if (IMG_Init(IMG_INIT_PNG) != IMG_INIT_PNG) {
        cout << "Failed to initialize SDL_image: " << IMG_GetError() << endl;
        return;
    }

    // Load background texture
    backgroundTexture = IMG_LoadTexture(renderer, "assets/images/Background_Grass.jpeg");
    if (!backgroundTexture) {
        cout << "Failed to load background texture: " << IMG_GetError() << endl;
    }

    // Initialize random seed
    srand(static_cast<unsigned>(time(0)));

    // Set up timers
    startTime = SDL_GetTicks();
    lastSpawnTime = startTime;

    // Spawn Enemy_Hsieh at the start of the game
    spawnEnemyHsieh();

    // Set up waves of enemies
    waves.push({4, 0, 0, 5});
    waves.push({0, 3, 0, 5});
    waves.push({0, 0, 5, 5});
    waves.push({4, 2, 0, 6});
    waves.push({2, 2, 4, 7});
    waves.push({3, 3, 3, 7});
    waves.push({4, 3, 4, 7});

    spawnEnemies();
}

// Destructor
FinalGame::~FinalGame() {
    // Clean up enemies
    for (BaseEnemy* enemy : enemies) {
        delete enemy;
    }
    enemies.clear();

    // Free background texture
    if (backgroundTexture) {
        SDL_DestroyTexture(backgroundTexture);
        backgroundTexture = nullptr;
    }

    // Quit SDL subsystems
    TTF_Quit();
    IMG_Quit();
}

// Handle events from SDL
void FinalGame::handleEvent(SDL_Event& e) {
    if (e.type == SDL_QUIT) {
        extern Gamemode gamemode;
        gamemode = EXIT;  // Set game mode to exit
        cout << "Quit event detected. Exiting game." << endl;
    }

    if (e.type == SDL_KEYDOWN) {
        if (e.key.keysym.sym == SDLK_ESCAPE) {
            extern Gamemode gamemode;
            gamemode = EXIT;  // Exit game when ESC is pressed
            cout << "ESC key pressed. Exiting game." << endl;
        }
    }

    player.handleEvent(e);  // Pass event to the player for handling
}

// Update game logic
void FinalGame::update(double deltaTime) {
    Uint32 currentTime = SDL_GetTicks();  // Get current time

    if (!waves.empty() && currentTime >= lastSpawnTime + currentWave.duration * 1000) {
        spawnEnemies();
        lastSpawnTime = currentTime;
    }

    // Update player
    player.update(deltaTime);

    // Update each enemy
    for (BaseEnemy* enemy : enemies) {
        if (enemy->isAlive()) {
            enemy->update(deltaTime);
        }
    }

    // Handle collisions between bullets and enemies
    for (BaseEnemy* enemy : enemies) {
        for (auto& bullet : player.getBullets()) {
            if (enemy->isAlive() && bullet.isActive() &&
                checkCollision(bullet, *enemy)) {
                bullet.deactivate();
                int damage = 50;  // Bullet damage value
                enemy->takeDamage(damage);
            }
        }
    }

    // Handle collisions between player and enemies
    for (BaseEnemy* enemy : enemies) {
        if (enemy->isAlive() && checkPlayerCollision(player, *enemy)) {
            resolvePlayerEnemyCollision(enemy);
        }
    }

    // Remove dead enemies
    enemies.erase(std::remove_if(enemies.begin(), enemies.end(),
                                 [](BaseEnemy* enemy) {
                                     if (!enemy->isAlive()) {
                                         delete enemy;  // Free memory
                                         return true;
                                     }
                                     return false;
                                 }),
                  enemies.end());

    // Check if all enemies are defeated to trigger victory
    if (enemies.empty() && waves.empty()) {
        extern Gamemode gamemode;
        gamemode = VICTORY;
        playMusic("assets/sounds/Victory_BGM.mp3");
    }
}

// Render all game elements
void FinalGame::render(SDL_Renderer* renderer) {
    SDL_SetRenderDrawColor(renderer, 204, 204, 204, 255);  // Set background color
    SDL_RenderClear(renderer);

    // Render background
    if (backgroundTexture) {
        SDL_RenderCopy(renderer, backgroundTexture, NULL, NULL);
    }

    // Render player
    int mouseX, mouseY;
    SDL_GetMouseState(&mouseX, &mouseY);
    player.render(renderer, mouseX, mouseY);

    // Render enemies
    for (BaseEnemy* enemy : enemies) {
        if (enemy->isAlive()) {
            enemy->render(renderer);
        }
    }

    SDL_RenderPresent(renderer);  // Update screen
}

// Spawn Enemy_Hsieh
void FinalGame::spawnEnemyHsieh() {
    if (!hasSpawnedHsieh) { // Ensure it spawns only once
        enemies.push_back(new Enemy_Hsieh(mRenderer, SDL_GetTicks(), &player));
        hasSpawnedHsieh = true;
        cout << "Enemy_Hsieh spawned at the start of Final Game!" << endl;
    }
}

// Spawn waves of enemies
void FinalGame::spawnEnemies() {
    if (waves.empty()) return;

    currentWave = waves.front();
    waves.pop();

    printf("Spawning wave with %d Sum, %d Integral, and %d ANDGate enemies\n",
           currentWave.numEnemySum, currentWave.numEnemyIntegral, currentWave.numEnemyANDGate);

    for (int i = 0; i < currentWave.numEnemySum; ++i) spawnEnemySum();
    for (int i = 0; i < currentWave.numEnemyIntegral; ++i) spawnEnemyIntegral();
    for (int i = 0; i < currentWave.numEnemyANDGate; ++i) spawnEnemyANDGate();
}

void FinalGame::spawnEnemySum() {
    float degree = static_cast<float>(rand() % 360);
    float x = player.getX() + spawnDistance * std::cos(degree * M_PI / 180);
    float y = player.getY() + spawnDistance * std::sin(degree * M_PI / 180);
    enemies.push_back(new Enemy_Sum(mRenderer));
    enemies.back()->setPosition(x, y);
}

void FinalGame::spawnEnemyIntegral() {
    float degree = static_cast<float>(rand() % 360);
    float x = player.getX() + spawnDistance * std::cos(degree * M_PI / 180);
    float y = player.getY() + spawnDistance * std::sin(degree * M_PI / 180);
    enemies.push_back(new Enemy_Integral(mRenderer, &player));
    enemies.back()->setPosition(x, y);
}

void FinalGame::spawnEnemyANDGate() {
    bool left = (rand() % 2) == 0;
    float x = left ? -200 : SCREEN_WIDTH + 200;
    x += static_cast<float>(rand() % 100);
    float y = static_cast<float>(rand() % (SCREEN_HEIGHT - 200) + 100);
    enemies.push_back(new Enemy_ANDGate(mRenderer));
    enemies.back()->setPosition(x, y);
}


// Check collision between a bullet and an enemy
bool FinalGame::checkCollision(const Bullet& bullet, const BaseEnemy& enemy) const {
    float dx = bullet.getX() - enemy.getX();
    float dy = bullet.getY() - enemy.getY();
    float distanceSquared = dx * dx + dy * dy;
    float radiusSum = bullet.getRadius() + enemy.getRadius();
    return distanceSquared <= radiusSum * radiusSum;
}

// Check collision between player and an enemy
bool FinalGame::checkPlayerCollision(const Player& player, const BaseEnemy& enemy) const {
    float dx = player.getX() - enemy.getX();
    float dy = player.getY() - enemy.getY();
    float distanceSquared = dx * dx + dy * dy;
    float radiusSum = player.getRadius() + enemy.getRadius();
    return distanceSquared <= radiusSum * radiusSum;
}

// Resolve collision between player and an enemy
void FinalGame::resolvePlayerEnemyCollision(BaseEnemy* enemy) {
    player.takeDamage(2);  // Player takes damage
    cout << "Player collided with an enemy and took damage!" << endl;
}
