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
public:
    // Constructor and destructor
    Game(SDL_Renderer* renderer);
    ~Game();

    // Main game functions
    void handleEvent(SDL_Event& e);
    void render(SDL_Renderer* renderer);
    void update(double deltaTime);
private:
    float colorChangeFactor = 0.0f;
    
    void drawGrid(SDL_Renderer* renderer, int grid_size, float colorChangeFactor);

    vector<Enemy> enemies;  // 敵人列表

    // Player instance
    Player player;
};

#endif
