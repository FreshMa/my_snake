#pragma once

#include "snake.h"

#include <stdlib.h>

#define DEFAULT_BOARD_SIZE 20

class Board{
private:
    int width_;
    int height_;
    Pos food_;

public:
    Board(int w = DEFAULT_BOARD_SIZE, int h = DEFAULT_BOARD_SIZE):
        width_(w), 
        height_(h) {
            srand(time(NULL));
    }
    
    // 随机生成食物位置，并且要求不能生成在蛇身上
    void GenFood(Snake* s);
    Pos GenPos();

    Pos GetFoodPos() const {
        return food_;
    }
    int GetWidth() const {
        return width_;
    }
    int GetHeight() const {
        return height_;
    }
};
