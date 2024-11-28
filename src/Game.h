#ifndef GAME_H
#define GAME_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <vector>
#include "Button.h"
#include "Global.h"
#include "Enemy.h"

using namespace std;

class Game {
public:
    Game(SDL_Renderer* renderer);
    ~Game();
    void render(SDL_Renderer* renderer);
    void handleEvent(SDL_Event& e);
private:
    // Member variables for the game, e.g., grid, colorChangeFactor, etc.
    float colorChangeFactor = 0.0f;  // Color change factor for the grid
    
    // Other member variables (such as game objects, buttons, etc.)
    void drawGrid(SDL_Renderer* renderer, int window_width, int window_height, int grid_size, float colorChangeFactor);
};

#endif
