#include <ncurses.h>
#include "lib/Maps.h"
#include "lib/Snake.h"
#include "lib/Gates.h"
#include "lib/AdditionalWindow.h"
#include "lib/Stage.h"
#include <chrono>
#include <thread>


using namespace lib;

int main() {
    auto gameStartTime = std::chrono::steady_clock::now();  // 게임 시작 시간
    initscr();
    start_color();
    curs_set(0);
    noecho();

    // Initialize color pairs
    init_pair(1, COLOR_WHITE, COLOR_BLUE);    // Walls
    init_pair(2, COLOR_WHITE, COLOR_GREEN);   // Immune Walls
    init_pair(3, COLOR_WHITE, COLOR_RED);     // Snake Head
    init_pair(4, COLOR_WHITE, COLOR_MAGENTA); // Snake Body
    init_pair(5, COLOR_WHITE, COLOR_YELLOW);  // Growth Item
    init_pair(6, COLOR_WHITE, COLOR_CYAN);    // Poison Item
    init_pair(7, COLOR_WHITE, COLOR_WHITE);   // Opened Gate
    init_pair(8, COLOR_WHITE, COLOR_BLACK);
    init_pair(9, COLOR_WHITE, COLOR_GREEN);

    // Create and initialize the map and gate
    Stage stageManager;
    stageManager.init(1);
    Map *map = stageManager.getMap();
    Snake *snake = stageManager.getSnake();
    Gate gate;
    stageManager.setSnakeTeleport(&gate);
    refresh();

    // Create score board and mission windows
    ScoreboardWindow scoreboardWin(45, 0, map, snake);
    MissionWindow missionWin(45, 12, map, snake, stageManager.getMissions());

    // Game Start
    snake->gameStart();

    //Gate가 생성되면 false가 된다
    while (snake->getIsGaming()) {
        if (gate.getCanUse() == true) {
            gate.gateOpen(map);
        }

        snake->userInput();
        snake->move();

        auto currentChronoTime = std::chrono::steady_clock::now();
        int elapsedSec = std::chrono::duration_cast<std::chrono::seconds>(currentChronoTime - gameStartTime).count();
        if (elapsedSec >= 100) {
            mvprintw(0, 0, "Game Over! Time limit exceeded.");
            refresh();
            std::this_thread::sleep_for(std::chrono::seconds(3));
            break;
        }

        if (snake->getTeleporting() > 0) {
            snake->teleportingDecrease();
        } else if (snake->getTeleporting() == 0) {
            snake->teleportingDecrease();
            gate.gateClose(map);
            gate.setCanUse(true);
        }
        
        map -> updateTimedWalls();

        //게임이 시작하고 5초 뒤부터 아이템 생성 시작.
        static time_t lastSpawnTime = time(nullptr);
        time_t currentTime = time(nullptr);
        if (currentTime - lastSpawnTime >= 5) {
            map->spawnItems();
            lastSpawnTime = currentTime;
        }

        map->draw(); 
        scoreboardWin.refresh();
        missionWin.refresh();
        mvprintw(20, 50, "Time: %3d", 100 - elapsedSec);
        refresh();

        // Check missions and proceed to next stage if all missions are completed
        if (stageManager.checkMissions()) {
            stageManager.nextStage();
            map = stageManager.getMap();
            snake = stageManager.getSnake();
            gate.setCanUse(true);
            scoreboardWin = ScoreboardWindow(45, 0, map, snake);
            missionWin = MissionWindow(45, 12, map, snake, stageManager.getMissions());
        }

        usleep(snake->getCurrentTick() * 1000);
        snake->updateTickIfNeeded();
    }

    nodelay(stdscr, false);
    getch();
    endwin();
    return 0;
}