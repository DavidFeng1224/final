#ifndef AUDIOMANAGER_H
#define AUDIOMANAGER_H

#include <SDL2/SDL_mixer.h>
#include <string>

// 初始化與釋放音頻系統
void initializeAudio();
void cleanupAudio();

// 背景音樂控制
void playMusic(const std::string& musicPath, int loops = -1);
void stopMusic();

#endif
