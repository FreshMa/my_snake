#include "snake.h"
#include "board.h"

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

void Snake::Init(Board* b, const Pos& pos, int move_intval, const Direction& dir) {
    board_ = b;
    body_.emplace_front(pos);
    move_interval_ = std::chrono::milliseconds(move_intval);
    last_move_tp_ = std::chrono::steady_clock::now();
    cur_dir_ = dir;
}

void Snake::Move() {
    if (body_.size() == 0) {
            return;
    }
    // 如果距离上次移动时间太短，不执行本次操作
    if (std::chrono::steady_clock::now() - last_move_tp_ < move_interval_) {
        return;
    }
    
    auto dir = cur_dir_;
    if (!dir_sequence_.empty()) {
        dir = dir_sequence_.front();
        dir_sequence_.pop();
    }
    // 如果是初始状态，保持不变即可，等待用户输入方向
    if (dir == Direction::kNull) return;

    // 更新蛇的身体坐标
    { 
        // 计算新元素的位置
        Pos step_pos =getStepByDir(dir);
        Pos head = body_.front();
        //printf("before:%d, %d\n", head.x, head.y);
        head += step_pos;
        // 校验这个位置的合法性，看是否会触发蛇的死亡
        if (WillHitBoarder(head) || WillHitSelf(head)) {
            alive_ = false;
            return;
        }

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
    if (dir == Direction::kNull || dir >= Direction::kMax) return;
    // 这里需要判断一下设置的方向是否合法，不合法的情况有下面两种
    // 1. 与上一个缓冲的方向冲突
    // 2. 与上一个缓冲的方向重复
    auto last_dir = cur_dir_;
    if (!dir_sequence_.empty()) {
        last_dir = dir_sequence_.front();
    }
    if (isDirConflict(dir, last_dir) || dir == last_dir) {
        return;
    }

    dir_sequence_.push(dir);
}

// 新位置是否会与自己产生碰撞
bool Snake::WillHitSelf(const Pos& head) {
    if (body_.size() == 0) return false;

    auto iter = body_.begin();
    auto end = body_.end();
    end--;
    ++iter;
    while(iter != end) {
        if (iter->x == head.x && iter->y == head.y) {
            return true;
        }
        iter++;
    }
    return false;
}

// 新的位置是否会触碰到地图边缘
bool Snake::WillHitBoarder(const Pos& head) {
    if (head.x < 0 || head.x >= board_->GetHeight() || 
            head.y < 0 || head.y >= board_->GetWidth()) {
        return true;
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
