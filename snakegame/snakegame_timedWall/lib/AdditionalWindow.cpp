#include "AdditionalWindow.h"

namespace lib {

ScoreboardWindow::ScoreboardWindow(int startx, int starty, Map *map, Snake *snake)
    : map(map), snake(snake) {
    window = newwin(10, 20, starty, startx);
    box(window, 0, 0);
}

void ScoreboardWindow::refresh() {
    werase(window);
    box(window, 0, 0);
    mvwprintw(window, 1, 1, "Score Board");
    mvwprintw(window, 2, 1, "B: %d / %d", snake->getCurrentLength(), snake->getMaxLength());
    mvwprintw(window, 3, 1, "+: %d", snake->getGrowthItems());
    mvwprintw(window, 4, 1, "-: %d", snake->getPoisonItems());
    mvwprintw(window, 5, 1, "G: %d", snake->getGateUses());
    wrefresh(window);
}

MissionWindow::MissionWindow(int startx, int starty, Map *map, Snake *snake, std::vector<MissionData> missions)
    : map(map), snake(snake), missions(missions) {
    window = newwin(10, 20, starty, startx);
    box(window, 0, 0);
}

void MissionWindow::refresh() {
    werase(window);
    box(window, 0, 0);
    mvwprintw(window, 1, 1, "Mission");
    auto it = missions.begin();
    bool completed = true;

    for (int i = 0; it != missions.end(); it++, i++) {
        bool localCompleted = (*it).condition(map, snake);
        mvwprintw(window, 2 + i, 1, "%s: %d (%c)", (*it).title, (*it).goal, localCompleted ? 'v' : ' ');
        completed &= localCompleted;
    }
    this->completed = completed;
    wrefresh(window);
}

}