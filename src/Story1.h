#ifndef STORY1_H
#define STORY1_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <vector>
#include <string>

using namespace std;

class Story1 {
public:
    Story1(SDL_Renderer* renderer);
    ~Story1();

    void handleEvent(SDL_Event& e);
    void update(double deltaTime);
    void render(SDL_Renderer* renderer);

private:
    void wrapText(const string& text, int maxWidth);  // 換行處理函數

    SDL_Renderer* mRenderer;
    TTF_Font* font;
    SDL_Texture* backgroundTexture;
    SDL_Texture* Player_Diologue_Texture;
    SDL_Texture* Professor_Diologue_Texture;
    Mix_Music* backgroundMusic;
    vector<string> dialogue;
    vector<string> wrappedText;  // 儲存每行的文本
    size_t currentLine;
};

#endif // STORY1_H
