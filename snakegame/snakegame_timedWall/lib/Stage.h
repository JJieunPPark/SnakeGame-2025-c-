#ifndef STAGE_H
#define STAGE_H

#include <vector>
#include <functional>
#include "Maps.h"
#include "Snake.h"
#include "AdditionalWindow.h" // MissionData를 포함합니다.

namespace lib {

const int N = 4; // 스테이지 수

struct StageData {
    int (*map)[21][21]; // 이차원 배열 포인터
    int sz;
    int *snake; // 일차원 배열
    std::vector<MissionData> mission;

    std::vector<std::pair<int,int>> timedWalls;
    int timedWallPeriod;
};

extern int field[N][21][21];
extern int snakeBodies[N][6];
extern StageData stageData[N];

class Stage {
public:
    Stage();
    void init(int stage);
    void nextStage();
    int getCurrentStage() const;
    void drawMap();
    void spawnItems();
    Map* getMap();
    Snake* getSnake();
    void setSnakeTeleport(Gate* g);
    bool checkMissions();
    std::vector<MissionData> getMissions() const;

private:
    int currentStage;
    Map map;
    Snake snake;
    std::vector<MissionData> missions;
    void getStage(int stage, Map &map, Snake &snake, std::vector<MissionData> &missionData);
};
}

#endif // STAGE_H