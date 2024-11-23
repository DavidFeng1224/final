#ifndef MENU_H
#define MENU_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>  // 用於文字渲染
#include <string>

class Menu {
public:
    Menu(SDL_Renderer* renderer);
    ~Menu();

    void handleEvent(SDL_Event& e);
    void update();
    void render();
    bool running() const;  // 確認是否還在運行

private:
    SDL_Renderer* renderer;
    bool isRunning;

    SDL_Rect startButton, howToPlayButton, settingsButton, exitButton;
    TTF_Font* font;  // 宣告字型指標

    void renderButton(SDL_Rect& button, const std::string& text, bool isHovered, int borderThickness);
    void handleButtonClick(int x, int y);
    bool isMouseOverButton(const SDL_Rect& button);  // 判斷滑鼠是否在按鈕上
};

#endif // MENU_H
