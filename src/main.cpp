#define SDL_MAIN_HANDLED  // 告訴 SDL2 不要自動處理 main 函數
#include <SDL2/SDL.h>
#include "Menu.h"

#define WIDTH 800
#define HEIGHT 600

int main(int argc, char* argv[]) {
    // 初始化 SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        SDL_Log("SDL could not initialize! SDL_Error: %s", SDL_GetError());
        return 1;
    }

    // 創建視窗
    SDL_Window* window = SDL_CreateWindow("Game Menu", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WIDTH, HEIGHT, SDL_WINDOW_SHOWN);
    if (!window) {
        SDL_Log("Window could not be created! SDL_Error: %s", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    // 創建渲染器
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        SDL_Log("Renderer could not be created! SDL_Error: %s", SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    // 創建 Menu
    Menu menu(renderer);

    // 主遊戲循環
    bool isRunning = true;
    SDL_Event e;
    while (isRunning) {
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) {
                isRunning = false;
            } else {
                menu.handleEvent(e);
            }
        }

        // 清除渲染器
        SDL_RenderClear(renderer);

        // 更新和繪製菜單
        menu.update();
        menu.render();

        // 顯示渲染內容
        SDL_RenderPresent(renderer);
    }

    // 清理資源
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
