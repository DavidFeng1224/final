#include "AudioManager.h"
#include <iostream>

static Mix_Music* currentMusic = nullptr;
static bool isAudioInitialized = false;


void initializeAudio() {
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
        std::cerr << "Failed to initialize SDL_mixer: " << Mix_GetError() << std::endl;
        isAudioInitialized = false;
    } else {
        isAudioInitialized = true;
    }
}

void cleanupAudio() {
    if (currentMusic) {
        Mix_FreeMusic(currentMusic);
        currentMusic = nullptr;
    }
    if (isAudioInitialized) {
        Mix_CloseAudio();
        isAudioInitialized = false;
    }
}

void playMusic(const std::string& musicPath, int loops) {
    if (!isAudioInitialized) {
        std::cerr << "Audio system not initialized!" << std::endl;
        return;
    }

    if (currentMusic) {
        Mix_HaltMusic();
        Mix_FreeMusic(currentMusic);
        std::cout << "haha didewfje" << '\n';
        currentMusic = nullptr;
    }

    currentMusic = Mix_LoadMUS(musicPath.c_str());
    if (!currentMusic) {
        std::cerr << "Failed to load music: " << Mix_GetError() << std::endl;
        return;
    }

    if (Mix_PlayMusic(currentMusic, loops) == -1) {
        std::cerr << "Failed to play music: " << Mix_GetError() << std::endl;
    }
}

void stopMusic() {
    if (!isAudioInitialized) {
        std::cerr << "Audio system not initialized!" << std::endl;
        return;
    }

    if (currentMusic) {
        Mix_HaltMusic();
    }
}
