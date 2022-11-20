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
    Board(int w = DEFAULT_BOARD_SIZE, int h = DEFAULT_BOARD_SIZE):width_(w), 
        height_(h), food_(Pos()){
            srand(time(NULL));
        }
    
    bool HitBoarder(const Pos& p) {
        if (p.x < 0 || p.x >= height_ || p.y < 0 || p.y >= width_) {
            return true;
        }
        return false;
    }

    // 随机生成食物位置，并且要求不能生成在蛇身上
    void GenFood(Snake* s) {
        while(1) {
            Pos new_food = GenPos();
            if (s->IsFoodInSnake(new_food)) {
                continue;
            }
            food_ = new_food;
            break;
        }
    }

    Pos GenPos() {
         int x = rand()%width_;
        int y = rand()%height_;
        
        Pos pos(x,y);
        return pos;
    }

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
