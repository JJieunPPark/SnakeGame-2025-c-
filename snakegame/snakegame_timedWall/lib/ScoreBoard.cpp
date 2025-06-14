#include "ScoreBoard.h"
#include <ncurses.h>

void ScoreBoard::display() {
    mvprintw(starty, startx, "Score Board");
    mvprintw(starty + 1, startx, "B: %d / %d", snake->getCurrentLength(), snake->getMaxLength());
    mvprintw(starty + 2, startx, "+: %d", snake->getGrowthItems());
    mvprintw(starty + 3, startx, "-: %d", snake->getPoisonItems());
    mvprintw(starty + 4, startx, "G: %d", snake->getGateUses());
}

void ScoreBoard::updateTime(int seconds) {
    int minutes = seconds / 60;
    int sec = seconds % 60;
    mvprintw(10, 5, "Time: %02d:%02d", minutes, sec);  // 좌표는 상황에 맞게 조정 가능
}