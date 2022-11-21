#include "board.h"

bool Board::HitBoarder(const Pos& p) {
    if (p.x < 0 || p.x >= height_ || p.y < 0 || p.y >= width_) {
        return true;
    }
    return false;
}
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
     int x = rand()%width_;
    int y = rand()%height_;
    
    Pos pos(x,y);
    return pos;
}