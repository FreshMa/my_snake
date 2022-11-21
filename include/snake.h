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

    Pos getStepByDir(const Direction& dir);

    bool isDirConflict(Direction dir1, Direction dir2);
public:
    Snake():cur_dir_(Direction::kNull){}

    void Init(const Pos& pos, 
              int move_intval = 500, 
              const Direction& dir = Direction::kNull);

    // 核心方法，移动蛇的位置。具体实现是删除链表尾部的元素，并在头部添加一个元素
    // 添加元素的位置基于当前头部位置以及方向确定
    void Move(Direction dir, int step = 1);
    // 设置蛇的运动方向
    void SetDirection(Direction dir);
    // 是否与自己产生了碰撞
    bool HasCollisionWithSelf();
    // 输出蛇的坐标
    void Show();
    // pos是否在蛇身体内
    bool IsFoodInSnake(const Pos& food);

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
};
