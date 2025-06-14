#ifndef SCOREBOARD_H
#define SCOREBOARD_H

#include <ncurses.h>
#include "Snake.h"

class ScoreBoard {
    Snake* snake;
    int startx;
    int starty;

public:
    ScoreBoard(Snake* snake, int startx, int starty)
        : snake(snake), startx(startx), starty(starty) {}

    void display(); // 선언만 합니다.
    void updateTime(int seconds);
};

#endif // SCOREBOARD_H