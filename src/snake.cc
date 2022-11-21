#include "snake.h"

Pos Snake::getStepByDir(const Direction& dir) {
    switch (dir)
    {
    case Direction::kUp:
        return Pos(-1, 0);
    case Direction::kDown:
        return Pos(1,0);
    case Direction::kLeft:
        return Pos(0, -1);
    case Direction::kRight:
        return Pos(0, 1);
    default:
        return Pos();
    }
}

bool Snake::isDirConflict(Direction dir1, Direction dir2) {
    if (dir1 == Direction::kUp && dir2 == Direction::kDown ||
        dir1 == Direction::kDown && dir2 == Direction::kUp ||
        dir1 == Direction::kLeft && dir2 == Direction::kRight ||
        dir1 == Direction::kRight && dir2 == Direction::kLeft) {
        return true;
    }
    return false;
}

void Snake::Init(const Pos& pos, int move_intval, const Direction& dir) {
    body_.emplace_front(pos);
    move_interval_ = std::chrono::milliseconds(move_intval);
    last_move_tp_ = std::chrono::steady_clock::now();
    cur_dir_ = dir;
}

void Snake::Move(Direction dir, int step) {
    if (body_.size() == 0 || 
        cur_dir_ == Direction::kNull ||
        cur_dir_ >= Direction::kMax) {
            return;
    }
    // 如果距离上次移动时间太短，不执行本次操作
    if (std::chrono::steady_clock::now() - last_move_tp_ < move_interval_) {
        return;
    }
    
    // 如果冲突，不改变当前行进方向
    if (isDirConflict(dir, cur_dir_)) {
        dir = cur_dir_;
    }
    while(step--) { 
        // 计算新元素的位置
        Pos step_pos =getStepByDir(dir);
        Pos head = body_.front();
        //printf("before:%d, %d\n", head.x, head.y);
        head += step_pos;
        //printf("after:%d, %d\n", head.x, head.y);
        // 剔除尾部元素，添加新的头部元素
        last_tail_pos_ = body_.back();
        body_.pop_back();
        body_.emplace_front(head);
    }
    cur_dir_ = dir;
    last_move_tp_ = std::chrono::steady_clock::now();
}

void Snake::SetDirection(Direction dir) {
    if (isDirConflict(dir, cur_dir_)) {
        return;
    }
    cur_dir_ = dir;
}

// 是否与自己产生了碰撞
bool Snake::HasCollisionWithSelf() {
    if (body_.size() == 0) return false;
    auto head = body_.begin();
    auto iter = head;
    ++iter;
    while(iter != body_.end()) {
        if (iter->x == head->x && iter->y == head->y) {
            return true;
        }
        iter++;
    }
    return false;
}

void Snake::Show() {
    /// for debug
    for(auto it = body_.begin(); it != body_.end(); ++it) {
        printf("{%d,%d}<-", it->x, it->y);
    }
    printf("\n");
}

// pos是否在蛇身体内
bool Snake::IsFoodInSnake(const Pos& food) {
    auto iter = body_.begin();
    while(iter != body_.end()) {
        if (food.x == iter->x && food.y == iter->y) {
            return true;
        }
        iter++;
    }
    return false;
}