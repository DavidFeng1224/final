#ifndef GLOBALS_H
#define GLOBALS_H

// 遊戲模式枚舉
enum Gamemode {
    MENU,
    STORY1,
    INGAME,
    STORY2,
    FINALGAME,
    VICTORY,
    GAMEOVER,
    INSTRUCTIONS,
    EXIT
};

// 全域遊戲模式變數
extern Gamemode gamemode;

// 屏幕尺寸常量
const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;

// Wave 結構定義
struct Wave {
    int numEnemySum;        // 敵人類型 Sum 的數量
    int numEnemyIntegral;   // 敵人類型 Integral 的數量
    int numEnemyANDGate;    // 敵人類型 ANDGate 的數量
    Uint32 duration;        // 此波敵人的持續時間（秒）
};

#endif
