#ifndef GLOBALS_H
#define GLOBALS_H

enum Gamemode {
    MENU,
    INGAME,
    VICTORY,
    LOSE,
    INSTRUCTIONS,
    EXIT
};

extern Gamemode gamemode;

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;

#endif
