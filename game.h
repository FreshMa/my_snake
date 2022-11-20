#pragma once

#include "snake.h"
#include "board.h"

#include <termios.h>
//#include <curses.h>
#include <unistd.h>

#include <vector>
#include <thread>
#include <atomic>
#include <mutex>
#include <condition_variable>
#include <chrono>

class Game{
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

    struct termios t_;

    void InputEnterOff() {
        tcgetattr(STDIN_FILENO, &t_);
        // 读取单个字符
        t_.c_lflag &= ~ICANON;
        // 关闭回显
        t_.c_lflag &= ~ECHO;
        // 隐藏光标
#ifdef HIDE_CURSOR
        printf("\e[?25l");
#endif
        tcsetattr(STDIN_FILENO, TCSANOW, &t_);
    }

    void InputEnterOn()
    {
        tcgetattr(STDIN_FILENO, &t_);
        t_.c_lflag |= ICANON;
        t_.c_lflag |= ECHO;
#ifdef HIDE_CURSOR
        printf("\e[?25h");
#endif
        tcsetattr(STDIN_FILENO, TCSANOW, &t_);
    }

    // 将getchar弄成非阻塞的
    // ref: https://stackoverflow.com/questions/448944/c-non-blocking-keyboard-input
    bool kbhit() {
        struct timeval tv{0, 0};
        fd_set fds;
        FD_ZERO(&fds);
        FD_SET(STDIN_FILENO, &fds);
        return select(1, &fds, NULL, NULL, &tv) > 0;
    }

    int max(int a, int b) {
        return a > b?a:b;
    }
public:
    SnakeGame():state_(0){}

    virtual void ShowUsage() {
        printf("Usage: use 'wsad' to move the snake\n");
    }
    
    virtual int Start() {
        Pos init_pos = board_.GenPos();
        snake_.Init(init_pos, 300);
        board_.GenFood(&snake_);

        board_img_.resize(board_.GetHeight());
        for(int i =0; i < board_img_.size(); ++i) {
            board_img_[i].resize(board_.GetWidth());
        }
        // 启动游戏
        state_.store(1);

        // 起一个线程去draw
        std::thread draw_thread(&SnakeGame::Draw, this);
        draw_thread.detach();

        // 接收键盘操作
        InputEnterOff();
        WaitForKeyboardEvent();

        // 退出
        state_.store(0);
        // 等待draw线程退出
        std::unique_lock<std::mutex> lk(mtx_);
        cv_.wait(lk);
        //printf("draw thread exit\n");
        InputEnterOn();
        return 0;
    }

    void WaitForKeyboardEvent() {
        while(1) {
            if (snake_.HasCollisionWithSelf() || board_.HitBoarder(snake_.GetHead())) {
                printf("YOU DIED, SCORE:%d\n", snake_.Length());
                //snake_.Show();
                break;
            }

            // 如果按下了键盘，那么读取字符，并设置方向
            if (kbhit()) {
                char c = getchar();
                switch (c) {
                    case 'w':
                    case 'W':
                        snake_.SetDirection(Direction::kUp);
                        break;
                    case 'a':
                    case 'A':
                        snake_.SetDirection(Direction::kLeft);
                        break;
                    case 's':
                    case 'S':
                        snake_.SetDirection(Direction::kDown);
                        break;
                    case 'd':
                    case 'D':
                        snake_.SetDirection(Direction::kRight);
                        break;
                    default:
                        break;
                }
            } 
            // 移动蛇的位置
            snake_.Move(snake_.GetDirection());
            
            // 如果蛇与食物相遇，吃掉当前食物，并且重新生成一个
            if (snake_.GetHead() == board_.GetFoodPos()) {
                snake_.Eat();
                board_.GenFood(&snake_);
            }
            // 不需要太实时
            std::this_thread::sleep_for(std::chrono::milliseconds(50));
        }
        state_.store(0);
    }


    // 异步不断draw棋盘
    void Draw() {
        while (state_.load() == 1)
        {
            // 把旧画面刷下去
            system("clear");
            DrawTitle();
            DrawFrame();
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
        cv_.notify_one();
    }

    void DrawTitle() {
        char buf[1024] = {0};
        // 1 先输出游戏标题
        // 计算要有多少个空格
        const char* title = "SNAKE GAME";
        int title_len = strlen(title);
        // 输出界面的时候，每一个.后面都有一个空格，所以实际界面的宽度是height的2倍
        int space_cnt = max(board_.GetHeight()*2-title_len, 0)/2;

        int i = 0;
        while(i < space_cnt) {
            buf[i] = ' ';
            ++i;
        }
        strncpy(buf+i, title, title_len);
        printf("%s\n", buf);

        // 2 在右侧输出成绩
        char score_buf[128] = {0};
        snprintf(score_buf, sizeof(score_buf), "SCORE: %3d", snake_.Length());

        memset(buf, 0, sizeof(buf));
        space_cnt = board_.GetHeight()*2 - strlen(score_buf) - 1;
        for(i = 0; i < space_cnt; ++i) {
            buf[i] = ' ';
        }
        strncpy(buf+i,score_buf, strlen(score_buf));
        printf("%s\n", buf);
        printf("\n");
    }

    void DrawFrame() {
        // 先初始化棋盘
        int w = board_.GetWidth();
        int h = board_.GetHeight();
        for(int i = 0; i < w; ++i) {
            for(int j = 0; j < h; ++j) {
                board_img_[i][j] = '.';
            }
        }
        // 绘制蛇
        std::list<Pos> body = snake_.GetBody();
        for(auto iter:body) {
            if (iter.x >= 0 && iter.x < board_.GetHeight() && 
            iter.y >= 0 && iter.y < board_.GetWidth()) {
                board_img_[iter.x][iter.y] = '0';
            }
        }

        // 绘制食物
        Pos food = board_.GetFoodPos();
        board_img_[food.x][food.y] = '*';

        // 绘制棋盘
        for(int i = 0; i < w; ++i) {
            for(int j = 0; j < h; ++j) {
                printf("%c ", board_img_[i][j]);
            }
            printf("\n");
        }
    }
};
