#pragma once
#include <vector>
#include <map>
#include <set>

class Light{
    public:
    Light(int, int); //id, time
    std::map<int,int> starting;
    std::map<int,int> schedule;
    int id; int T;
    int cycle = 0;
    std::vector<char> used;
    int differenceInCycle;
    //std::set <int> streets;
    void addStreet(int);
    bool isGreen(int streetId, int time);
    bool canCross(int);
    void addToSchedule(int, int);
    void reset();
};
