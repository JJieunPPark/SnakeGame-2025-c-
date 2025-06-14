#include "Gates.h"

using namespace std;
using namespace lib;

//생성자
Gate::Gate(){
    init();
}

//랜덤값 초기화 함수
void Gate::init(){
    srand(time(NULL));
}

//Map* map은 맵을 나타내는 객체 포인터이다. map -> setTile()로 맵에 접근한다.
void Gate::gateOpen(Map *map){ //gate 2개 벽에 랜덤하게 생성.
    int d;
    int x{0}, y{0};
    int attempts = 0;

    do{ //gate A 생성
        d = rand() % 4;

        if(d == 0){ // right wall
            x = 20;
            y = rand() % 21;
        }
        else if(d == 1){ // left wall
            x = 0;
            y = rand() % 21;
        }
        else if(d == 2){ // up wall
            x = rand() % 21;
            y = 0;
        }
        else if(d == 3){ // down wall
            x = rand() % 21;
            y = 20;
        }
        attempts++;
        if(attempts > 100 ) return;
    }
    while(map -> getTile(x, y) == ElementType::ImmuneWall); //ImuneWall이면 좌표 다시 설정

    gateA[0] = x;
    gateA[1] = y;
    gateA[2] = d; // 방향(0~3), 오른쪽,왼쪽,위,아래 

    attempts = 0;

    do{ //gate B 생성
        d = rand() % 4;

        if(d == 0){ // right wall
            x = 20;
            y = rand() % 21;
        }
        else if(d == 1){ // left wall
            x = 0;
            y = rand() % 21;
        }
        else if(d == 2){ // up wall
            x = rand() % 21;
            y = 0;
        }
        else if(d == 3){ // down wall
            x = rand() % 21;
            y = 20;
        }
        attempts++;
        if(attempts > 100) return;
    }
    while(map -> getTile(x, y) == ElementType::ImmuneWall || (gateA[0] == x && gateA[1] == y)); //gateA랑 다른 위치 위해서 같은 좌표는 거부한다.

    gateB[0] = x;
    gateB[1] = y;
    gateB[2] = d;

    //Map의 해당 위치를 OpenedGate(숫자 7)에 해당하는 값으로 설정한다.
    //setTile은 Map에서 온 함수
    map -> setTile(gateA[0], gateA[1], ElementType::OpenedGate);
    map -> setTile(gateB[0], gateB[1], ElementType::OpenedGate);
    
    //재생성 방지용, 이후에 다시 사용할 수 없도록 상태를 설정.
    canUse = false;
}

//두개의 Gate를 다시 원래 벽 상태(1)로 되돌린다.
void Gate::gateClose(Map* map){
    map -> setTile(gateA[0], gateA[1], ElementType::Wall);
    map -> setTile(gateB[0], gateB[1], ElementType::Wall);
}

//Gate를 생성할 수 있는 상태인지 여부를 저장하는 변수
bool Gate::getCanUse(){
    return canUse;
}
void Gate::setCanUse(bool b){
    canUse = b;
}

//snake가 Gate에 진입했을 떄 호출한다.
//snake가 어디로 이동할지 정보 제공 -> out[]값 통해서
void Gate::teleport(int x, int y, int out[]){
    if(x == gateA[0] && y == gateA[1]){
        out[0] = gateB[0];
        out[1] = gateB[1];
        out[2] = gateB[2];
    }
    else{
        out[0] = gateA[0];
        out[1] = gateA[1];
        out[2] = gateA[2];
    }
}