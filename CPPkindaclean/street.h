#pragma once

#include <map>

class Street {
public:
    int start, end, id, length;
    float situation = 0;
    int useness = 0;
    std::map<int, float> history;
    float waitingTime = 0;
    int passed = 0;
    int entered = 0;
    Street(int, int, int, int);
    int carLeave();
    int carEnter();
    void reset();


};