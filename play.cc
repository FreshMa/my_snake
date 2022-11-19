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
    SnakeGame sg;
    Player p(&sg);
    p.Play();
    return 0;
}