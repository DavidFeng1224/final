// Screen.h
#ifndef SCREEN_H
#define SCREEN_H

class Screen {
public:
    virtual void display() = 0; // 純虛擬函式，強制子類別實作
    virtual ~Screen() = default; // 虛擬解構函式
};

#endif // SCREEN_H
