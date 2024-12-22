#include "FinalGame.h"
#include "AudioManager.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>
#include <iostream>
#include <cmath>
#include <algorithm>
#include <queue>
#include <cstdlib>
#include <ctime>

using namespace std;

// 構造函式
FinalGame::FinalGame(SDL_Renderer* renderer)
    : player(renderer, 100), mRenderer(renderer), hasSpawnedHsieh(false),
      spawnDistance(600.0f), enemyHsiehAlive(true) {
    // 初始化 SDL_ttf
    if (TTF_Init() == -1) {
        cout << "Failed to initialize SDL_ttf: " << TTF_GetError() << endl;
        return;
    }

    // 初始化 SDL_image
    if (IMG_Init(IMG_INIT_PNG) != IMG_INIT_PNG) {
        cout << "Failed to initialize SDL_image: " << IMG_GetError() << endl;
        return;
    }

    // 載入背景紋理
    backgroundTexture = IMG_LoadTexture(renderer, "assets/images/Background_Grass.jpeg");
    if (!backgroundTexture) {
        cout << "Failed to load background texture: " << IMG_GetError() << endl;
    }

    // 初始化隨機數種子
    srand(static_cast<unsigned>(time(0)));

    // 記錄開始時間
    startTime = SDL_GetTicks();
    lastSpawnTime = startTime;

    // 初始生成特殊敵人 Enemy_Hsieh
    spawnEnemyHsieh();
}
void FinalGame::render(SDL_Renderer* renderer) {
    SDL_SetRenderDrawColor(renderer, 204, 204, 204, 255); // 設定背景顏色
    SDL_RenderClear(renderer);

    // 繪製背景
    if (backgroundTexture) {
        SDL_RenderCopy(renderer, backgroundTexture, NULL, NULL);
    }

    // 獲取滑鼠位置
    int mouseX, mouseY;
    SDL_GetMouseState(&mouseX, &mouseY);

    // 繪製玩家
    player.render(renderer, mouseX, mouseY);

    // 繪製敵人
    for (BaseEnemy* enemy : enemies) {
        if (enemy->isAlive()) {
            enemy->render(renderer);
        }
    }

    SDL_RenderPresent(renderer); // 更新屏幕
}


// 析構函式
FinalGame::~FinalGame() {
    for (BaseEnemy* enemy : enemies) {
        delete enemy; // 釋放記憶體
    }
    enemies.clear();

    if (backgroundTexture) {
        SDL_DestroyTexture(backgroundTexture);
    }

    TTF_Quit();
    IMG_Quit();
}

// 更新遊戲邏輯
void FinalGame::update(double deltaTime) {
    Uint32 currentTime = SDL_GetTicks();

    // 每 5 秒生成敵人
    if (enemyHsiehAlive && currentTime >= lastSpawnTime + 5000) {
        for (int i = 0; i < 2; ++i) {
            spawnEnemySum();
            spawnEnemyIntegral();
            spawnEnemyANDGate();
        }
        lastSpawnTime = currentTime;
    }

    // 更新玩家
    player.update(deltaTime);

    // 更新敵人
    for (BaseEnemy* enemy : enemies) {
        if (enemy->isAlive()) {
            enemy->update(deltaTime);
            // 檢測並處理玩家與敵人的碰撞
            if (checkPlayerCollision(player, *enemy)) {
                resolvePlayerEnemyCollision(enemy);
            }
        }
    }

    IsWin = true;
    // 處理子彈與敵人的碰撞
    for (BaseEnemy* enemy : enemies) {
        for (auto& bullet : player.getBullets()) {
            if (enemy->isAlive() && bullet.isActive() &&
                checkCollision(bullet, *enemy)) {
                bullet.deactivate();
                enemy->takeDamage(50);

                // 如果是 Enemy_Hsieh，檢查是否死亡
                if (dynamic_cast<Enemy_Hsieh*>(enemy) && !enemy->isAlive()) {
                    enemyHsiehAlive = false;
                }
            }
        }
        if(enemy->isAlive()){
            IsWin = false;
        }
    }

    // 移除已死亡的敵人
    enemies.erase(remove_if(enemies.begin(), enemies.end(),
                            [](BaseEnemy* enemy) {
                                if (!enemy->isAlive()) {
                                    delete enemy;
                                    return true;
                                }
                                return false;
                            }),
                  enemies.end());

    if(IsWin && !enemyHsiehAlive){
        extern Gamemode gamemode;  // 引用全域變數
        gamemode = VICTORY;
        playMusic("assets/sounds/Victory_BGM.mp3");
    }
}

// 生成特殊敵人 Enemy_Hsieh
void FinalGame::spawnEnemyHsieh() {
    if (!hasSpawnedHsieh) {
        enemies.push_back(new Enemy_Hsieh(mRenderer, SDL_GetTicks(), &player));
        hasSpawnedHsieh = true;
    }
}

// 生成 Sum 類型敵人
// void FinalGame::spawnEnemySum() {
//     float degree = static_cast<float>(rand() % 360);
//     float x = player.getX() + spawnDistance * cos(degree * M_PI / 180);
//     float y = player.getY() + spawnDistance * sin(degree * M_PI / 180);
//     enemies.push_back(new Enemy_Sum(mRenderer));
//     enemies.back()->setPosition(x, y);
// }

// 生成 Integral 類型敵人
void FinalGame::spawnEnemyIntegral() {
    float degree = static_cast<float>(rand() % 360);
    float x = player.getX() + spawnDistance * cos(degree * M_PI / 180);
    float y = player.getY() + spawnDistance * sin(degree * M_PI / 180);
    enemies.push_back(new Enemy_Integral(mRenderer, &player));
    enemies.back()->setPosition(x, y);
}

// 生成 ANDGate 類型敵人
void FinalGame::spawnEnemyANDGate() {
    bool left = (rand() % 2) == 0;
    float x = left ? -200 : SCREEN_WIDTH + 200;
    float y = static_cast<float>(rand() % (SCREEN_HEIGHT - 200) + 100);
    enemies.push_back(new Enemy_ANDGate(mRenderer));
    enemies.back()->setPosition(x, y);
}


void FinalGame::handleEvent(SDL_Event& e) {
    if (e.type == SDL_QUIT) {
        extern Gamemode gamemode;
        gamemode = EXIT; // 切換到退出模式
    }

    if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_ESCAPE) {
        extern Gamemode gamemode;
        gamemode = EXIT; // 切換到退出模式
    }

    player.handleEvent(e); // 傳遞事件給玩家處理
}

// 碰撞檢測
bool FinalGame::checkCollision(const Bullet& bullet, const BaseEnemy& enemy) const {
    float dx = bullet.getX() - enemy.getX();
    float dy = bullet.getY() - enemy.getY();
    float distanceSquared = dx * dx + dy * dy;
    float radiusSum = bullet.getRadius() + enemy.getRadius();
    return distanceSquared <= radiusSum * radiusSum;
}

// 檢查玩家與敵人的碰撞
bool FinalGame::checkPlayerCollision(const Player& player, const BaseEnemy& enemy) const {
    float dx = player.getX() - enemy.getX();
    float dy = player.getY() - enemy.getY();
    float distanceSquared = dx * dx + dy * dy;
    float radiusSum = player.getRadius() + enemy.getRadius();
    return distanceSquared <= radiusSum * radiusSum;
}

bool FinalGame::isPositionOverlapping(float x, float y, float radius) const {
    for (const auto* enemy : enemies) {
        if (enemy->isAlive()) {
            float dx = enemy->getX() - x;
            float dy = enemy->getY() - y;
            float distanceSquared = dx * dx + dy * dy;
            float combinedRadius = enemy->getRadius() + radius;

            if (distanceSquared < combinedRadius * combinedRadius) {
                return true; // 發生重疊
            }
        }
    }
    return false; // 沒有重疊
}

void FinalGame::spawnEnemySum() {
    float x, y;
    int maxAttempts = 100; // 最多嘗試次數，避免無限迴圈
    int attempts = 0;

    do {
        float degree = static_cast<float>(rand() % 360);
        x = player.getX() + spawnDistance * cos(degree * M_PI / 180);
        y = player.getY() + spawnDistance * sin(degree * M_PI / 180);
        attempts++;
    } while (isPositionOverlapping(x, y, 30.0f) && attempts < maxAttempts);

    if (attempts < maxAttempts) {
        enemies.push_back(new Enemy_Sum(mRenderer));
        enemies.back()->setPosition(x, y);
    } else {
        std::cerr << "Failed to spawn Enemy_Sum after " << maxAttempts << " attempts!" << std::endl;
    }
}


// 處理玩家與敵人的碰撞
void FinalGame::resolvePlayerEnemyCollision(BaseEnemy* enemy) {
    if (!enemy || !enemy->isAlive()) return; // 確保敵人有效且存活

    // 計算方向向量（從敵人指向玩家）
    float dx = player.getX() - enemy->getX();
    float dy = player.getY() - enemy->getY();
    float distance = std::sqrt(dx * dx + dy * dy);

    if (distance < 0.0001f) return; // 避免零向量導致的錯誤

    // 設定方向單位向量
    float dirX = dx / distance;
    float dirY = dy / distance;

    // 讓玩家彈開
    player.startBounce(dirX, dirY);

    // 同時讓敵人稍微移動以避免重疊（反方向）
    float overlap = player.getRadius() + enemy->getRadius() - distance;
    if (overlap > 0) {
        enemy->setPosition(
            enemy->getX() - dirX * overlap * 0.5f, // 移動一半的重疊距離
            enemy->getY() - dirY * overlap * 0.5f
        );
    }

    // 玩家受到傷害
    player.takeDamage(2);
}

