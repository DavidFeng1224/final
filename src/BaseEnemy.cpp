#include "BaseEnemy.h"

void BaseEnemy::render(SDL_Renderer* renderer) {
    // 默認渲染行為（例如，留空或顯示佔位）
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);  // 顯示紅色佔位
    SDL_Rect rect = {static_cast<int>(x), static_cast<int>(y), 50, 50};
    SDL_RenderFillRect(renderer, &rect);
}
