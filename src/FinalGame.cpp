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
#include "Global.h"
#include "Player.h"

using namespace std;

// Constructor
FinalGame::FinalGame(SDL_Renderer* renderer)
    : player(renderer, 100), mRenderer(renderer) {
    // 初始化 SDL_ttf
    if (TTF_Init() == -1) {
        cout << "Failed to initialize SDL_ttf: " << TTF_GetError() << endl;
        return;
    }

    // 初始化 SDL_image 用於圖片加載
    if (IMG_Init(IMG_INIT_PNG) != IMG_INIT_PNG) {
        cout << "Failed to initialize SDL_image: " << IMG_GetError() << endl;
        return;
    }

    // 加載背景材質
    backgroundTexture = IMG_LoadTexture(renderer, "assets/images/Background_Grass.jpeg");
    if (!backgroundTexture) {
        cout << "Failed to load background texture: " << IMG_GetError() << endl;
    }

    // 初始化計時器
    startTime = SDL_GetTicks();
    lastSpawnTime = startTime;

    // 初始化隨機數種子
    srand(static_cast<unsigned>(time(0)));

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
    // 清理敵人對象
    for (BaseEnemy* enemy : enemies) {
        delete enemy;
    }
    enemies.clear();

    // 釋放背景材質
    if (backgroundTexture) {
        SDL_DestroyTexture(backgroundTexture);
        backgroundTexture = nullptr;
    }

    // 終止 SDL 子系統
    TTF_Quit();
    IMG_Quit();
}

// Handle events from SDL
void FinalGame::handleEvent(SDL_Event& e) {
    if (e.type == SDL_QUIT) {
        extern Gamemode gamemode;
        gamemode = EXIT;  // 設定遊戲模式為退出
        cout << "Quit event detected. Exiting game." << endl;
    }

    if (e.type == SDL_KEYDOWN) {
        if (e.key.keysym.sym == SDLK_ESCAPE) {
            extern Gamemode gamemode;
            gamemode = EXIT;  // 按下 ESC 鍵退出遊戲
            cout << "ESC key pressed. Exiting game." << endl;
        }
    }

    player.handleEvent(e);  // 傳遞事件給玩家進行處理
}

void FinalGame::constrainPlayerToBounds() {
    player.setPosition(
        std::clamp(player.getX(), static_cast<float>(player.getRadius()), static_cast<float>(SCREEN_WIDTH - player.getRadius())),
        std::clamp(player.getY(), static_cast<float>(player.getRadius()), static_cast<float>(SCREEN_HEIGHT - player.getRadius()))
    );
}


// Update game logic
void FinalGame::update(double deltaTime) {
    Uint32 currentTime = SDL_GetTicks();  // 獲取當前時間

    if (!waves.empty() && currentTime >= lastSpawnTime + currentWave.duration * 1000) {
        spawnEnemies();
        lastSpawnTime = currentTime;
    }

    // 更新玩家
    player.update(deltaTime);

    // 更新每個敵人
    for (BaseEnemy* enemy : enemies) {
        if (enemy->isAlive()) {
            enemy->update(deltaTime);
        }
    }

    // 處理子彈與敵人的碰撞
    for (BaseEnemy* enemy : enemies) {
        for (auto& bullet : player.getBullets()) {
            if (enemy->isAlive() && bullet.isActive() &&
                checkCollision(bullet, *enemy)) {
                bullet.deactivate();
                int damage = 50;  // 子彈傷害值
                enemy->takeDamage(damage);
            }
        }
    }

    // 處理玩家與敵人的碰撞
    for (BaseEnemy* enemy : enemies) {
        if (enemy->isAlive() && checkPlayerCollision(player, *enemy)) {
            resolvePlayerEnemyCollision(enemy);
        }
    }

    // 處理敵人之間的碰撞
    for (size_t i = 0; i < enemies.size(); ++i) {
        if(!enemies[i]->isAlive()){
            enemyLeft--;
        }
        for (size_t j = i + 1; j < enemies.size(); ++j) {
            if (enemies[i]->isAlive() && enemies[j]->isAlive() &&
                checkEnemyCollision(*enemies[i], *enemies[j])) {
                resolveEnemyOverlap(enemies[i], enemies[j]);
            }
        }
    }
    if(enemyLeft == 0){
        extern Gamemode gamemode;  // 引用全域變數
        gamemode = VICTORY;
        playMusic("assets/sounds/Victory_BGM.mp3");
    }
    // 檢查是否需要生成 Enemy_Hsieh
    if (currentTime >= startTime + 20000) {  // 超過 20 秒
        bool noOtherEnemiesAlive = std::none_of(enemies.begin(), enemies.end(), [](BaseEnemy* enemy) {
            return enemy->isAlive();
        });

        bool hsiehNotSpawned = std::none_of(enemies.begin(), enemies.end(), [](BaseEnemy* enemy) {
            return dynamic_cast<Enemy_Hsieh*>(enemy);
        });

        if (noOtherEnemiesAlive && hsiehNotSpawned) {
            enemies.push_back(new Enemy_Hsieh(mRenderer, startTime + 20000, &player));
            std::cout << "Spawning Enemy_Hsieh!" << std::endl;
        }
    }

    // 移除已死亡的敵人
    enemies.erase(std::remove_if(enemies.begin(), enemies.end(),
                                 [](BaseEnemy* enemy) {
                                     if (!enemy->isAlive()) {
                                         delete enemy;  // 釋放記憶體
                                         return true;
                                     }
                                     return false;
                                 }),
                  enemies.end());
}

// Render all game elements
void FinalGame::render(SDL_Renderer* renderer) {
    SDL_SetRenderDrawColor(renderer, 204, 204, 204, 255);  // 設定背景顏色
    SDL_RenderClear(renderer);

    // 渲染背景
    if (backgroundTexture) {
        SDL_RenderCopy(renderer, backgroundTexture, NULL, NULL);
    }

    int mouseX, mouseY;
    SDL_GetMouseState(&mouseX, &mouseY);  // Get the current mouse position
    // 渲染玩家
    player.render(renderer, mouseX, mouseY);

    // 渲染敵人
    for (BaseEnemy* enemy : enemies) {
        if (enemy->isAlive()) {
            enemy->render(renderer);
        }
    }

    SDL_RenderPresent(renderer);  // 更新畫面
}

void FinalGame::resolvePlayerEnemyCollision(BaseEnemy* enemy) {
    static int bounceStep = 10;  // 彈開的初始步驟數
    while (checkPlayerCollision(player, *enemy) && bounceStep > 0) {
        float deltaX = enemy->getX() - player.getX();
        float deltaY = enemy->getY() - player.getY();
        float distance = std::sqrt(deltaX * deltaX + deltaY * deltaY);

        if (distance == 0.0f) {
            deltaX = 1.0f;  // 防止除以零
            deltaY = 0.0f;
            distance = 1.0f;
        }

        float overlap = player.getRadius() + enemy->getRadius() - distance;

        if (overlap > 0) {
            float dirX = deltaX / distance;
            float dirY = deltaY / distance;
            float bounceDistance = std::max(static_cast<float>(bounceStep), overlap);

            // 更新玩家位置
            player.setPosition(
                player.getX() - dirX * bounceDistance,
                player.getY() - dirY * bounceDistance
            );

            // 更新敵人位置
            enemy->setPosition(
                enemy->getX() + dirX * bounceDistance,
                enemy->getY() + dirY * bounceDistance
            );

            // 限制玩家和敵人位置在螢幕範圍內
            constrainPlayerToBounds();
            constrainEnemyToBounds(enemy);
        }

        bounceStep--;
    }

    // 玩家扣血
    player.takeDamage(2);
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

// Generate enemies at random positions
void FinalGame::spawnEnemies() {
    currentWave = waves.front();
    waves.pop();

    printf("Spawning wave with %d Sum, %d Integral, and %d ANDGate enemies\n",
           currentWave.numEnemySum, currentWave.numEnemyIntegral, currentWave.numEnemyANDGate);

    for (int i = 0; i < currentWave.numEnemySum; ++i) spawnEnemySum();
    for (int i = 0; i < currentWave.numEnemyIntegral; ++i) spawnEnemyIntegral();
    for (int i = 0; i < currentWave.numEnemyANDGate; ++i) spawnEnemyANDGate();
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

// Check collision between two enemies
bool FinalGame::checkEnemyCollision(const BaseEnemy& enemy1, const BaseEnemy& enemy2) const {
    float dx = enemy1.getX() - enemy2.getX();
    float dy = enemy1.getY() - enemy2.getY();
    float distanceSquared = dx * dx + dy * dy;
    float radiusSum = enemy1.getRadius() + enemy2.getRadius();
    return distanceSquared <= radiusSum * radiusSum;
}

// Resolve overlap between two enemies
void FinalGame::resolveEnemyOverlap(BaseEnemy* enemy1, BaseEnemy* enemy2) {
    float dx = enemy1->getX() - enemy2->getX();
    float dy = enemy1->getY() - enemy2->getY();
    float distance = std::sqrt(dx * dx + dy * dy);

    if (distance == 0.0f) {
        dx = 1.0f;
        dy = 0.0f;
        distance = 1.0f;
    }

    float radiusSum = enemy1->getRadius() + enemy2->getRadius();
    float overlap = radiusSum - distance;

    float moveFactor = overlap / 2.0f;
    dx /= distance;
    dy /= distance;

    enemy1->setPosition(
        enemy1->getX() + dx * moveFactor,
        enemy1->getY() + dy * moveFactor);

    enemy2->setPosition(
        enemy2->getX() - dx * moveFactor,
        enemy2->getY() - dy * moveFactor);
}

void FinalGame::constrainEnemyToBounds(BaseEnemy* enemy) {
    enemy->setPosition(
        std::clamp(enemy->getX(), static_cast<float>(enemy->getRadius()), static_cast<float>(SCREEN_WIDTH - enemy->getRadius())),
        std::clamp(enemy->getY(), static_cast<float>(enemy->getRadius()), static_cast<float>(SCREEN_HEIGHT - enemy->getRadius()))
    );
}

