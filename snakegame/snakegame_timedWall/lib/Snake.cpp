#include "Snake.h"

using namespace lib;

SnakeStack::SnakeStack() {}

void SnakeStack::init(int* positions, int length) {
    tail = length - 1;
    for (int i = 0; i < length; i++) {
        arr[i][0] = positions[i * 2];
        arr[i][1] = positions[i * 2 + 1];
    }
}

void SnakeStack::moveto(int x, int y) {
    for (int i = tail; i > 0; i--) {
        arr[i][0] = arr[i - 1][0];
        arr[i][1] = arr[i - 1][1];
    }
    arr[0][0] = x;
    arr[0][1] = y;
}

void SnakeStack::getHeadPosition(int out[]) {
    out[0] = arr[0][0];
    out[1] = arr[0][1];
}

void SnakeStack::drawSnakePart(Map* map) {
    for (int i = 0; i <= tail; i++) {
        if (i == 0) {
            map->setTile(arr[i][0], arr[i][1], ElementType::SnakeHead);
        } else {
            map->setTile(arr[i][0], arr[i][1], ElementType::SnakeBody);
        }
    }
}

void SnakeStack::eraseTail(Map* map) {
    map->setTile(arr[tail][0], arr[tail][1], ElementType::Empty);
}

void SnakeStack::snakeLonger() {
    if (tail < 440) {
        tail++;
        arr[tail][0] = arr[tail - 1][0];
        arr[tail][1] = arr[tail - 1][1];
    }
}

void SnakeStack::snakeShorter(Map* map) {
    if (tail > 0) {
        map->setTile(arr[tail][0], arr[tail][1], ElementType::Empty);
        tail--;
    }
}

Snake::Snake(int length, Map* m, Gate* g, Direction d)
    : snakeLength(length), map(m), gate(g), direction(d), growthItems(0), poisonItems(0), gateUses(0), maxLength(length), gameTime(0) {
    snake.init(nullptr,0);
}

void Snake::pickUpGrowthItem() {
    longer();
    increaseGrowthItems();
}

void Snake::pickUpPoisonItem() {
    shorter();
    increasePoisonItems();
}

void Snake::init(int* positions) {
   if (positions){
    snake.init(positions,3);
   }
   //점수 초기화하기 위해서.
    snakeLength = 3;
    maxLength = 3;
    growthItems = 0;
    poisonItems = 0;
    gateUses = 0;
    teleporting = 0;
}

void Snake::gameStart() {
    isGaming = true;
}

void Snake::userInput() {
    keypad(stdscr, true);
    nodelay(stdscr, true);
    int key = getch();
    switch (key) {
    case KEY_RIGHT:
        setDirection(Direction::Right);
        break;
    case KEY_LEFT:
        setDirection(Direction::Left);
        break;
    case KEY_UP:
        setDirection(Direction::Up);
        break;
    case KEY_DOWN:
        setDirection(Direction::Down);
        break;
    case 113:
        isGaming = false;
        break;
    }
}

void Snake::gameEnd() {
    isGaming = false;
}

bool Snake::getIsGaming() {
    return isGaming;
}

void Snake::setGate(Gate* g){
    gate = g;
}

int Snake::getTeleporting() {
    return teleporting;
}

void Snake::teleportingDecrease() {
    teleporting--;
}

SnakeStack* Snake::getSnakeStack() {
    return &snake;
}

void Snake::position(int out[]) {
    int temp[2];
    snake.getHeadPosition(temp);
    out[0] = temp[0];
    out[1] = temp[1];
}

void Snake::setDirection(Direction d) {
    direction = d;
}

void Snake::move() {
    int temp[2]{};
    snake.getHeadPosition(temp);
    switch (direction) {
    case Direction::Right:
        temp[0] += 1;
        break;
    case Direction::Left:
        temp[0] -= 1;
        break;
    case Direction::Up:
        temp[1] -= 1;
        break;
    case Direction::Down:
        temp[1] += 1;
        break;
    }
    ElementType elem;
    elem = map->getTile(temp[0], temp[1]);

    if (elem == ElementType::Wall || elem == ElementType::ImmuneWall || elem == ElementType::SnakeBody || elem == ElementType::TimedWall) {
        isGaming = false;
    } else if (elem == ElementType::OpenedGate) {
        snake.eraseTail(map);
        int next[3]{0,};
        gate->teleport(temp[0], temp[1], next);
        teleporting = snakeLength;
        switch (next[2]) {
        case 0: // Right Wall
            snake.moveto(next[0] - 1, next[1]);
            setDirection(Direction::Left);
            break;
        case 1: // Left Wall
            snake.moveto(next[0] + 1, next[1]);
            setDirection(Direction::Right);
            break;
        case 2: // Up Wall
            snake.moveto(next[0], next[1] + 1);
            setDirection(Direction::Down);
            break;
        case 3: // Down Wall
            snake.moveto(next[0], next[1] - 1);
            setDirection(Direction::Up);
            break;
        }
        increaseGateUses(); // 게이트 사용 횟수 증가
        drawSnake();
    } else {
        if (elem == ElementType::GrowthItem) {
            pickUpGrowthItem();
            map->removeItem(temp[0], temp[1]);
        } else if (elem == ElementType::PoisonItem) {
            pickUpPoisonItem();
            map->removeItem(temp[0], temp[1]);
        } else if (elem == ElementType::SlowItem) {
            isSlowed = true;
            slowItemStartTime = std::chrono::steady_clock::now();
            currentTick = baseTick * 2;  // 예: 평소보다 2배 느리게
            map->removeItem(temp[0], temp[1]);
        }
        snake.eraseTail(map);
        snake.moveto(temp[0], temp[1]);
        drawSnake();
    }
}

void Snake::drawSnake() {
    snake.drawSnakePart(map);
}

void Snake::longer() {
    snakeLength++;
    snake.snakeLonger();
    if (teleporting > 0) {
        teleporting++;
    }
    if (snakeLength > maxLength) {
        maxLength = snakeLength;
    }
}

void Snake::shorter() {
    if (snakeLength > 3) {
        snakeLength--;
        snake.snakeShorter(map);
        if (teleporting > 0) {
            teleporting--;
        }
    } else {
        gameEnd();
    }
}

int Snake::getCurrentLength() const {
    return snakeLength;
}

int Snake::getMaxLength() const {
    return maxLength;
}

int Snake::getGrowthItems() const {
    return growthItems;
}

int Snake::getPoisonItems() const {
    return poisonItems;
}

int Snake::getGateUses() const {
    return gateUses;
}

int Snake::getGameTime() const {
    return gameTime;
}

void Snake::increaseGrowthItems() {
    growthItems++;
}

void Snake::increasePoisonItems() {
    poisonItems++;
}

void Snake::increaseGateUses() {
    gateUses++;
}

void Snake::updateGameTime(int time) {
    gameTime = time;
}
void Snake::updateTickIfNeeded() {
    if (isSlowed) {
        auto now = std::chrono::steady_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(now - slowItemStartTime).count();
        if (duration > 2000) {
            isSlowed = false;
            currentTick = baseTick;
        }
    }
}