#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <termios.h>
#include <curses.h>
#include <unistd.h>
#include <pthread.h>
using namespace std;

struct Point{
    int x;
    int y;
};

struct SnakeBlock{
    Point pos;
    struct SnakeBlock* prev;
    struct SnakeBlock* next;

    SnakeBlock(){
        pos.x = -1;
        pos.y = -1;
    }
    SnakeBlock(Point p):pos(p){}
};

enum MOVE_DIR{
        NOT_INIT = 0,
        LEFT = 1,
        RIGHT = 2,
        UP = 3,
        DOWN = 4
};

MOVE_DIR get_dir() {
    char c = getchar();
    //printf("char:%c\n", c);
    int ex = 0;
    switch(c) {
        case 'w':
            return UP;
        case 'a':
            return LEFT;
        case 's':
            return DOWN;
        case 'd':
            return RIGHT;
        default:
            return NOT_INIT;
    }
}

void* snake_cb(void* arg);
class Snake{
  public:
    friend class Board;
    Snake(int x, int y) {
        Point pos{x, y};
        init(pos);
    }
    Snake(Point pos) {
        init(pos);
    }
    ~Snake() {
        while(_head) {
            SnakeBlock *tmp = _head;
            _head = _head->next;
            delete tmp;
            tmp = nullptr;
        }
    }

    Snake(const Snake&) = delete;
    Snake& operator=(const Snake&)=delete;

    bool is_dead(){
        return _state == 1;
    }
    int length() {
        return _len;
    }

    int set_dir(MOVE_DIR dir) {
        switch(dir) {
            case UP:
                if (_dir != DOWN) _dir = UP;
                break;
            case DOWN:
                if (_dir != UP) _dir = DOWN;
                break;
            case RIGHT:
                if (_dir != LEFT) _dir = RIGHT;
                break;
            case LEFT:
                if (_dir != RIGHT) _dir = LEFT;
                break;
            default:
                _dir = NOT_INIT;
        }
        return 0;
    }

    void show() {
        SnakeBlock *t = _head;
        while(t) {
            printf("{%d, %d}", t->pos.x, t->pos.y);
            if (t->next) {
                printf("->");
            } else {
                printf("\n");
            }
            t = t->next;
        }
    }

    Point cur_pos() {
        if (_head) {
            return _head->pos;
        }
        return Point{-1,-1};
    }

  private:
    void init(Point p) {
        SnakeBlock *h = new SnakeBlock(p);
        h->prev = nullptr;
        h->next = nullptr;
        _head = h;

        _len = 1;
        _dir = UP;
        _state = 0;
        _food = {-1,-1};
        _food_eaten = false;

        pthread_t tid;
        //pthread_create(&tid, NULL, snake_cb, this);
    }

  public:
    void set_status(int s) {
        _state = s;
    }

    void set_food(Point food) {
        _food = food;
    }

    int move() {
        if (_len == 0) {return -1;}

        SnakeBlock* t = new SnakeBlock();
        //printf("cur dir:%d\n", _dir);

        switch(_dir) {
            case UP:
                t->pos.y = _head->pos.y+1;
                t->pos.x = _head->pos.x;
                break;
            case DOWN:
                t->pos.y = _head->pos.y-1;
                t->pos.x = _head->pos.x;
                break;
            case RIGHT:
                t->pos.x = _head->pos.x+1;
                t->pos.y = _head->pos.y;
                break;
            case LEFT:
                t->pos.x = _head->pos.x-1;
                t->pos.y = _head->pos.y;
                break;
            default:
                delete t;
                return -1;
        }
        
        // add this node to head
        t->next = _head;
        _head->prev = t;
        _head = t;

        // if it's food, nice, eat it
        if (_food.x != -1 && t->pos.x == _food.x && t->pos.y == _food.y) {
            _food_eaten = true;
            _len++;
            _food.x = -1;
            _food.y = -1;
        } else { // oops, delete last node
            t = _head;
            while(t && t->next) {
                t = t->next;
            }
            if (t) {
                t->prev->next = nullptr;
                t->prev = nullptr;
                delete t;
            };
            _food_eaten = false;
        }

        //check if dead
        if (is_hit_self()) {
            printf("hit self, died\n");
            _state = 1;
            return -1;
        }
        
        return 0;
    }

    bool is_hit_self() {
        if (!_head) return false;
        SnakeBlock* t = _head->next;
        while(t) {
            if (t->pos.x == _head->pos.x && t->pos.y == _head->pos.y) {
                return true;
            }
            t = t->next;
        }
        return  false;
    }

  private:
    SnakeBlock* _head;
    int _len;
    MOVE_DIR _dir;
    int _state; // 0-normal, 1-dead
    Point _food;
    bool _food_eaten;
};


void* snake_cb(void* arg) {
    Snake *s = (Snake*)arg;
    while(1) {
        if (s->is_dead()) break;
        MOVE_DIR dir = get_dir();
        s->set_dir(dir);
    }
    return nullptr;
}


class Board{
  public:
    Board(int x, int y, Snake* s):_s(s){
        _x = x; _y = y;
        srand(time(NULL));
    }
    ~Board() {
    }
    
    void fill() {
        static bool first = true;
        for (int i = 0; i < _x; ++i) {
            for (int j = 0; j < _y; ++j) {
                if (!_s->_food_eaten && _board[i][j] == 2) continue;
                _board[i][j] = 0;
            }
        }

        SnakeBlock *t = _s->_head;
        while(t) {
            //printf("[SNAKE] x:%d, y:%d\n", t->pos.x, t->pos.y);
            _board[t->pos.x][t->pos.y] = 1;
            t = t->next;
        }
        
        // generate food when first gen board or food has beed eaten
        if (first || _s->_food_eaten) {
            Point food = gen_food();
            _s->set_food(food);
            _board[food.x][food.y] = 2;
            first = false;
        }
    }

    // print current board
    void show() {
        for(int i = 0; i < 50; ++i) {
            printf("\n");
        }
        for (int j = _y-1; j >=0; --j) {
            for (int i=0; i <_x; ++i) {
                char c = '.'; // empty
                if (_board[i][j] == 1) { // snake
                    c = '0';
                } else if (_board[i][j] == 2) { // food
                    c = '*';
                }
                printf("%c ", c);
            }
            printf("\n");
        }
    }

    Point gen_food() {
        Point p;
        // food block should not be one of snake block
        while(1) {
            int x = rand()%_x;
            int y = rand()%_y;
            bool hit = false;
            if (_s) {
                SnakeBlock *t = _s->_head;
                while(t) {
                    if (t->pos.x == x && t->pos.y == y) {
                        hit = true;
                        break;
                    }
                    t = t->next;
                }
            }
            p.x = x;
            p.y = y;
            if (!hit) break;
        }
        return p;
    }

    bool is_hit_boarder() {
        if (!_s) return false;
        Point cur_head = _s->cur_pos();
        if (cur_head.x >= _x || cur_head.y >= _y ||
                cur_head.x < 0 || cur_head.y < 0) {
            return true;
        }
        return false;
    }

  private:
    Snake *_s;
    int _x, _y;

    int _board[1000][1000]; 
};


struct termios t;

void input_enter_off()
{
    tcgetattr(STDIN_FILENO, &t);
    t.c_lflag &= ~ICANON;
    tcsetattr(STDIN_FILENO, TCSANOW, &t);
}

void input_enter_on()
{
    tcgetattr(STDIN_FILENO, &t);
    t.c_lflag |= ICANON;
    tcsetattr(STDIN_FILENO, TCSANOW, &t);
}

int main() {
    //testSnake();
    Snake s(4,4);
    Board b(10,10, &s);
    input_enter_off();

    while(true) {
#if 1
        s.set_dir(get_dir());
#endif
        b.fill();
        s.move();
        if (s.is_dead()) {
            printf("YOU DIED, HIT SELF\n");
            break;
        }

        if (b.is_hit_boarder()) {
            printf("YOU DIED, HIT BORADER\n");
            break;
        }
        b.show();

        usleep(5*1000);
    }
    printf("Score:%d\n", s.length());

    return 0;
}
