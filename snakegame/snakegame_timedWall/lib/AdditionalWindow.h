#ifndef ADDITIONALWINDOW_H
#define ADDITIONALWINDOW_H

#include "Maps.h"
#include "Snake.h"
#include <vector>
#include <functional>
#include <ncurses.h>

namespace lib {

struct MissionData {
    const char* title;
    int goal;
    std::function<bool(Map*, Snake*)> condition;
};

class ScoreboardWindow {
    Map *map;
    Snake *snake;
    WINDOW *window;
    time_t startTime;

public:
    ScoreboardWindow(int startx, int starty, Map *map, Snake *snake);
    void refresh();
    void updateTimer();
};

class MissionWindow {
    Map *map;
    Snake *snake;
    std::vector<MissionData> missions;
    bool completed;
    WINDOW *window;
public:
    MissionWindow(int startx, int starty, Map *map, Snake *snake, std::vector<MissionData> missions);
    void refresh();
    bool isComplete() { return completed; }
    void setMissions(std::vector<MissionData> missions) { this->missions = missions; return; }
};

}

#endif // ADDITIONALWINDOW_H