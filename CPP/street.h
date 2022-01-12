#pragma once
#include <map>

class Street{
    public:
    int start, end;
    int id; int length;
    float situation = 0;
    int useness = 0;
    std::map<int, float> history;
    float waitingTime = 0;

    int passed = 0;
    int entered = 0;

    Street(int, int, int, int);
    bool is_empty();

    int carLeave();
    int carEnter();

    void reset();


};