#include "board.h"

void Board::GenFood(Snake* s) {
    while(1) {
        Pos new_food = GenPos();
        if (s->IsFoodInSnake(new_food)) {
            continue;
        }
        food_ = new_food;
        break;
    }
}

Pos Board::GenPos() {
    int x = rand()%height_;
    int y = rand()%width_;
    
    Pos pos(x,y);
    return pos;
}