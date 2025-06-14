#ifndef ENUMS_H
#define ENUMS_H

namespace lib {
    enum ElementType {
        Empty = 0,
        Wall = 1,
        ImmuneWall = 2,
        SnakeHead = 3,
        SnakeBody = 4,
        GrowthItem = 5,
        PoisonItem = 6,
        OpenedGate = 7,
        SlowItem= 8,
        TimedWall = 9,
    };

    enum Direction{
        Right, Left, Up, Down,
    };
}

#endif // ENUMS_H