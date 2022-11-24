#pragma once
#include <list>
#include <chrono>
#include <cstdio>
#include <queue>

class Board;

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
    // 移动方向队列
    std::queue<Direction> dir_sequence_;
    Direction cur_dir_;
    // 蛇尾的上一个位置，用于增长长度
    Pos last_tail_pos_;
    // 蛇的移动速度
    std::chrono::milliseconds move_interval_;
    // 蛇上次的移动时间点
    std::chrono::time_point<std::chrono::steady_clock> last_move_tp_;

    Board* board_;
    bool alive_;

    Pos getStepByDir(const Direction& dir);

    bool isDirConflict(Direction dir1, Direction dir2);

public:
    Snake():cur_dir_(Direction::kNull), alive_(true){}

    void Init(Board* b, const Pos& pos, 
              int move_intval = 500, 
              const Direction& dir = Direction::kNull);
    bool IsAlive() {
        return alive_;
    }
    // 核心方法，控制蛇的移动，更新蛇的身体坐标
    void Move();
    // 设置蛇的运动方向，实际是向方向队列中添加一个合法的方向
    void SetDirection(Direction dir);
    // 当蛇头移动到这个位置时，是否会与自己产生碰撞
    bool WillHitSelf(const Pos& head);
    // 当蛇头移动到这个位置时，是否会碰到地图边缘
    bool WillHitBoarder(const Pos& head);
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
