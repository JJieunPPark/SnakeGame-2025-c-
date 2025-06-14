#include "Maps.h"
#include <ncurses.h>
#include <cstdlib>
#include <ctime>
#include <chrono>

namespace lib {

// 맵 사이즈 21x21로 고정, numItems: 맵에 존재하는 아이템 수 (최대 3)
Map::Map()
    : sz(21)
    , numItems(0)
    , lastSpawnedItem(ElementType::PoisonItem)
    , timedWallPeriod(0)
    , timedWallTickCounter(0)
{
    map = new int*[sz];
    for (int i = 0; i < sz; i++) {
        map[i] = new int[sz]{};
    }
    std::srand(std::time(nullptr));
}

// 2차원 배열 해제
Map::~Map() {
    for (int i = 0; i < sz; i++) {
        delete[] map[i];
    }
    delete[] map;
}

// stage 데이터 기반으로 맵 초기화
void Map::initialize(int (*stage)[21][21]) {
    for (int i = 0; i < sz; i++) {
        for (int j = 0; j < sz; j++) {
            map[i][j] = (*stage)[i][j];
        }
    }

    for(auto &p : timedWalls){
        int x = p.first, y=p.second;
        map[y][x] = ElementType::TimedWall;
    }
}

// 맵 출력
void Map::draw() {
    for (int i = 0; i < sz; i++) {
        for (int j = 0; j < sz; j++) {
            switch (map[i][j]) {
                case ElementType::Wall:
                    attron(COLOR_PAIR(1));
                    mvprintw(i, j * 2, "  ");
                    attroff(COLOR_PAIR(1));
                    break;
                case ElementType::ImmuneWall:
                    attron(COLOR_PAIR(2));
                    mvprintw(i, j * 2, "  ");
                    attroff(COLOR_PAIR(2));
                    break;
                case ElementType::SnakeHead:
                    attron(COLOR_PAIR(3));
                    mvprintw(i, j * 2, "  ");
                    attroff(COLOR_PAIR(3));
                    break;
                case ElementType::SnakeBody:
                    attron(COLOR_PAIR(4));
                    mvprintw(i, j * 2, "  ");
                    attroff(COLOR_PAIR(4));
                    break;
                case ElementType::GrowthItem:
                    attron(COLOR_PAIR(5));
                    mvprintw(i, j * 2, "GI");
                    attroff(COLOR_PAIR(5));
                    break;
                case ElementType::PoisonItem:
                    attron(COLOR_PAIR(6));
                    mvprintw(i, j * 2, "PI");
                    attroff(COLOR_PAIR(6));
                    break;
                case ElementType::OpenedGate:
                    attron(COLOR_PAIR(7));
                    mvprintw(i, j * 2, "  ");
                    attroff(COLOR_PAIR(7));
                    break;
                case ElementType::SlowItem:
                    attron(COLOR_PAIR(8)); // 기존 색상 재활용
                    mvprintw(i, j * 2, "SI");
                    attroff(COLOR_PAIR(8));
                    break;
                case ElementType::TimedWall:
                    attron(COLOR_PAIR(1));   // 일반 Wall과 비슷한 색
                    mvprintw(i, j * 2, "  "); 
                    attroff(COLOR_PAIR(1));
                    break;
                default:
                    mvprintw(i, j * 2, "  ");
                    break;
            }
        }
    }
}

// 맵 타일 설정
void Map::setTile(int x, int y, ElementType elem) {
    map[y][x] = elem;
}

// 타일 값 가져오기
ElementType Map::getTile(int x, int y) {
    return static_cast<ElementType>(map[y][x]);
}

// 1) 타임드/무빙 월 초기화
void Map::initTimedWalls(const std::vector<std::pair<int,int>>& initialTimed, int periodTicks)
{
    // 1) 전달받은 좌표 목록 복사
    timedWalls = initialTimed;

    // 2) 토글 주기와 카운터 초기화
    timedWallPeriod     = periodTicks;
    timedWallTickCounter = 0;

    // 3) 맵 배열에 TimedWall 타일을 찍는다
    for (auto &p : timedWalls) {
        int x = p.first;
        int y = p.second;
        map[y][x] = ElementType::TimedWall;
    }
}


void Map::updateTimedWalls() {
    if (timedWalls.empty()) return;
    timedWallTickCounter++;
    if (timedWallTickCounter < timedWallPeriod) return;
    // 일정 틱이 지나면 상태 변경
    timedWallTickCounter = 0;
    for (auto &p : timedWalls) {
        int x = p.first, y = p.second;
        if (map[y][x] == ElementType::TimedWall) {
            map[y][x] = ElementType::Empty;  // 사라짐
        } else if (map[y][x] == ElementType::Empty) {
            map[y][x] = ElementType::TimedWall; // 나타남
        }
        // (다른 타입이면 변화시키지 않도록)
    }
    timedWallTickCounter = 0;
}



// 아이템 생성
void Map::spawnItems() {
    using namespace std::chrono;

    auto now = steady_clock::now();
    auto growthItemDuration = duration_cast<seconds>(now - growthItemSpawnTime).count();
    auto poisonItemDuration = duration_cast<seconds>(now - poisonItemSpawnTime).count();

    if (numItems < 3 || growthItemDuration >= 10 || poisonItemDuration >= 10) {
        // 기존 아이템 모두 제거
        for (int y = 0; y < sz; y++) {
            for (int x = 0; x < sz; x++) {
                if (map[y][x] == ElementType::GrowthItem ||
                    map[y][x] == ElementType::PoisonItem ||
                    map[y][x] == ElementType::SlowItem) {
                    map[y][x] = ElementType::Empty;
                }
            }
        }

        numItems = 0;

        // GrowthItem
        {
            int x, y;
            do {
                x = rand() % (sz - 2) + 1;
                y = rand() % (sz - 2) + 1;
            } while (map[y][x] != ElementType::Empty);
            map[y][x] = ElementType::GrowthItem;
            growthItemSpawnTime = now;
            numItems++;
        }

        // PoisonItem
        {
            int x, y;
            do {
                x = rand() % (sz - 2) + 1;
                y = rand() % (sz - 2) + 1;
            } while (map[y][x] != ElementType::Empty);
            map[y][x] = ElementType::PoisonItem;
            poisonItemSpawnTime = now;
            numItems++;
        }

        // SlowItem
        {
            int x, y;
            do {
                x = rand() % (sz - 2) + 1;
                y = rand() % (sz - 2) + 1;
            } while (map[y][x] != ElementType::Empty);
            map[y][x] = ElementType::SlowItem;
            numItems++;
        }
    }
}

// 아이템 제거
void Map::removeItem(int x, int y) {
    map[y][x] = ElementType::Empty;
    numItems--;
}

// SnakeBody 여부 확인
bool Map::isSnakeBody(int x, int y) {
    return map[y][x] == ElementType::SnakeBody;
}

}
