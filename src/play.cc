#include "game.h"

class Player{
public:
    Player(Game* g):g_(g){}
    void Play(){
        g_->ShowUsage();
        g_->Start();
    }
private:
    Game* g_;
};

int main() {
    // 可以指定地图的宽和高
    SnakeGame sg(30,20);
    Player p(&sg);
    p.Play();
    return 0;
}