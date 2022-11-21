#pragma once
#include <list>
#include <chrono>
#include <cstdio>

struct Pos{
    int x;
    int y;

    Pos(int x = -1, int y = -1):x(x), y(y){}
    Pos& operator+=(const Pos& p) {
        this->x += p.x;
        this->y += p.y;
        return *this;
    }

    bool operator==(const Pos& p) {
        return this->x == p.x & this->y == p.y;
    }
};

enum class Direction{
    kNull = 0,
    kUp = 1,
    kDown = 2,
    kLeft = 3,
    kRight = 4,
    kMax
};

class Snake{
private:
    // 链表形式保存蛇的身体
    std::list<Pos> body_;
    // 当前的移动方向
    Direction cur_dir_;
    // 蛇尾的上一个位置，用于增长长度
    Pos last_tail_pos_;
    // 蛇的移动速度
    std::chrono::milliseconds move_interval_;
    // 蛇上次的移动时间点
    std::chrono::time_point<std::chrono::steady_clock> last_move_tp_;

    Pos getStepByDir(const Direction& dir) {
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

    bool isDirConflict(Direction dir1, Direction dir2) {
        if (dir1 == Direction::kUp && dir2 == Direction::kDown ||
            dir1 == Direction::kDown && dir2 == Direction::kUp ||
            dir1 == Direction::kLeft && dir2 == Direction::kRight ||
            dir1 == Direction::kRight && dir2 == Direction::kLeft) {
            return true;
        }
        return false;
    }
public:
    Snake():cur_dir_(Direction::kNull), 
        last_tail_pos_(Pos()){}

    void Init(const Pos& pos, 
              int move_intval = 500, 
              const Direction& dir = Direction::kNull) {
        body_.emplace_front(pos);
        move_interval_ = std::chrono::milliseconds(move_intval);
        last_move_tp_ = std::chrono::steady_clock::now();
        cur_dir_ = dir;
    }
    // 获取当前蛇头所在的位置
    Pos GetHead() const {
        return body_.front();
    }

    std::list<Pos> GetBody() const {
        return body_;
    }

    Direction GetDirection() const {
        return cur_dir_;
    }
    // 吃到food的时候调用这个方法
    void Eat() {
        body_.emplace_back(last_tail_pos_);
    }

    int Length() const {
        return body_.size();
    }

    // 核心方法，移动蛇的位置。具体实现是删除链表尾部的元素，并在头部添加一个元素
    // 元素的位置基于当前头部位置以及方向确定
    void Move(Direction dir, int step = 1) {
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

    void SetDirection(Direction dir) {
        if (isDirConflict(dir, cur_dir_)) {
            return;
        }
        cur_dir_ = dir;
    }

    // 是否与自己产生了碰撞
    bool HasCollisionWithSelf() {
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

    void Show() {
        /// for debug
        for(auto it = body_.begin(); it != body_.end(); ++it) {
            printf("{%d,%d}<-", it->x, it->y);
        }
        printf("\n");
    }

    // pos是否在蛇身体内
    bool IsFoodInSnake(const Pos& food) {
        auto iter = body_.begin();
        while(iter != body_.end()) {
            if (food.x == iter->x && food.y == iter->y) {
                return true;
            }
            iter++;
        }
        return false;
    }

};
