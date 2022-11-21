#pragma once

#include "snake.h"
#include "board.h"

#include <termios.h>

#include <vector>
#include <thread>
#include <atomic>
#include <mutex>
#include <condition_variable>
#include <chrono>

// 控制终端的光标展示，字符读取方式等
class ConsoleGameHelper{
private:
    struct termios t_;
public:
    ConsoleGameHelper();

    ~ConsoleGameHelper();
};

class Game{
private:
    ConsoleGameHelper helper_;
public:
    // 会阻塞在start函数里边
    virtual int Start() = 0;
    virtual void ShowUsage() = 0;
};

class SnakeGame: public Game{
private:
    Snake snake_;
    Board board_;

    std::atomic<int> state_;
    std::mutex mtx_;
    std::condition_variable cv_;

    using VisableBoard = std::vector<std::vector<char>>;
    VisableBoard board_img_;

    int max(int a, int b) {
        return a > b?a:b;
    }
    // 等待键盘输入并移动蛇
    void WaitForKeyboardEvent();
    // 检测键盘是否有输入
    bool kbhit();

    // 异步不断draw棋盘
    void Draw();
    void DrawTitle();
    void DrawFrame();
public:
    SnakeGame():state_(0){}

    virtual void ShowUsage() override {
        printf("Usage: use 'wsad' to move the snake\n");
    }
    
    virtual int Start() override;
};
