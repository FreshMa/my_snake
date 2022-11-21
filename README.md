# snake game
toy tool

## operate
use `wasd` to move

## ref

thanks to [https://github.com/gogakoreli/snake](https://github.com/gogakoreli/snake)

## screenshots
cmd line based, so it's simple
```bash
               SNAKE GAME
                             SCORE:  12

. . . . . . . . . . . . . . . . . . . . 
. . . . . . . . . . . . . . . . . . 0 0 
. . . . . . . . . . . . . . . . . . . 0 
. . . . . . . . . . . . . . . . . . . 0 
. . . . . . . . . . . . . . . . . . . 0 
. . . . . . . . . . . . . . . . . . . 0 
. . . . . . . . . . . . . . . . . . . 0 
. . . . . . . . . . . . . . . . . . . 0 
. . . . . . . . . . . . . . . . . . 0 0 
. . . . . . . . . . . . . . . . . . 0 0 
. . . . . . . . . . . . . . . . . . . . 
. . . . . . . . . . . . . . . . . . . . 
. . . . . . . . . . . . . . . . . . . . 
. . . . . . . . . . . . . . . . . . . . 
. . . . * . . . . . . . . . . . . . . . 
. . . . . . . . . . . . . . . . . . . . 
. . . . . . . . . . . . . . . . . . . . 
. . . . . . . . . . . . . . . . . . . . 
. . . . . . . . . . . . . . . . . . . . 
. . . . . . . . . . . . . . . . . . . . 
YOU DIED, SCORE:12
```


## todo
1. When died, the length of snake in the screen may be shorter by 1 than expected, because the snake head overlaps with body/boarder
2. When moving snake too quickly, the snake may die unexpectly(press left, up, right quickly, the snake may not respond to up, which caused the self collision)
3. snake head and body are difficult to distinguish

