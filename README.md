# snake game
toy tool

## build & run
`make && ./src/snake`

## operate
use `wasd` to move

## ref

thanks to [https://github.com/gogakoreli/snake](https://github.com/gogakoreli/snake)

## screenshots
cmd line based, so it's simple

The upper left corner is the coordinate origin

The x-axis is from top to bottom and the y-axis is from left to right.

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
1. [FIXED] When died, the length of snake in the screen may be shorter by 1 than expected, because the snake head overlaps with body/boarder
2. [FIXED] When moving snake too quickly, the snake may die unexpectly(press left, up, right quickly, the snake may not respond to up, which caused the self collision)
3. snake head and body are difficult to distinguish

