#ifndef GATES_H
#define GATES_H

#include <stdlib.h>
#include <time.h>
#include "Enums.h"
#include "Maps.h"

using namespace lib;

class Gate{
    private:
    int gateA[3]{0,}; // gate[x, y, wall direction]
    int gateB[3]{0,};
    bool canUse = true; //gate 생성할 수 있는 상태인지 처음에는 true

    public:
    Gate();
    void init(); // random setting
    void gateOpen(Map* map); // Open two Gates
    void gateClose(Map* map); // Close two Gates
    void teleport(int x, int y, int out[]);
    bool getCanUse();
    void setCanUse(bool b);
    void reset();
};


#endif //GATE_H