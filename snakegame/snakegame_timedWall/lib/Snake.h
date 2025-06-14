#ifndef SNAKE_H
#define SNAKE_H

#include <unistd.h>
#include <ncurses.h>
#include "Enums.h"
#include "Maps.h"
#include "Gates.h"

using namespace lib;

class SnakeStack {
private:
    int arr[441][2] = {}; // arr[length](21*21)[position](x, y)
    int tail = -1; // snake tail index

public:
    SnakeStack(); 
    void init(int* positions, int length);
    void moveto(int x, int y); // move snake
    void getHeadPosition(int out[]); // return position(x, y) of head
    void drawSnakePart(Map* map); // draw all part of snake
    void eraseTail(Map* map); // erase trace of tail in map after move

    // Methods to make item effects
    void snakeLonger(); // +1 snakelength
    void snakeShorter(Map* map); // -1 snakelength
};

class Snake {
private:
    int snakeLength;
    SnakeStack snake;
    Map* map;
    Gate* gate;
    Direction direction;
    bool isGaming = false;
    int teleporting;

    int growthItems = 0; //////////////////// Growth 아이템 획득 개수
    int poisonItems = 0; //////////////////// Poison 아이템 획득 개수
    int gateUses = 0; /////////////////////// 게이트 사용 횟수
    int maxLength = 0; ////////////////////// 최대 길이
    int gameTime = 0; /////////////////////// 게임 시간 (초 단위)
    bool isSlowed = false;
    std::chrono::steady_clock::time_point slowItemStartTime;
    int baseTick = 200;
    int currentTick = 200;

public:
    Snake(int length, Map* m, Gate* g ,Direction d = Direction::Left);

    void init(int* positions);

    void gameStart(); // game start
    void userInput(); // get input while gaming
    void gameEnd(); // game end

    bool getIsGaming(); // return isGaming
    void setGate(Gate* g);
    int getTeleporting();
    void teleportingDecrease();
    
    SnakeStack* getSnakeStack(); // return snakestack pointer
    void position(int out[]); // return head position
    void setDirection(Direction d); // Change Direction
    void move(); // move to direction
    void drawSnake(); // draw Snake in map

    // Methods to make item effects
    void longer(); // +1 snakeLength
    void shorter(); // -1 snakeLength

    void processItem();
    void pickUpGrowthItem();
    void pickUpPoisonItem();

    // Methods to track score
    int getCurrentLength() const;
    int getMaxLength() const;
    int getGrowthItems() const;
    int getPoisonItems() const;
    int getGateUses() const;
    int getGameTime() const;

    void increaseGrowthItems();
    void increasePoisonItems();
    void increaseGateUses();
    void updateGameTime(int time);
    void updateTickIfNeeded();
    int getCurrentTick() const {
        return currentTick;
    }
};

#endif // SNAKE_H