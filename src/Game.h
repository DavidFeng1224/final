#ifndef GAME_H
#define GAME_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <vector>
#include "Button.h"
#include "Global.h"
#include "Enemy.h"
#include "Player.h"

using namespace std;

class Game {
private:
    SDL_Texture* backgroundTexture = nullptr;  // 新增變數儲存背景圖片的紋理

    vector<Enemy> enemies;
    Player player;
public:
    Game(SDL_Renderer* renderer);
    ~Game();
    void handleEvent(SDL_Event& e);
    void render(SDL_Renderer* renderer);
    void update(double deltaTime);
};

#endif
