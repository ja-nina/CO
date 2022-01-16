#pragma once

#include <vector>
#include <unordered_map>
#include <set>
#include <map>

class Light {
public:
    std::unordered_map<int, int> starting;
    std::unordered_map<int, int> schedule;
    std::vector<int> usedStreets;
    std::vector<char> used;
    int numberOfUsedStreets = 0;
    int usage = 1;
    int cycle = 0;
    int id, T, differenceInCycle;
    Light(int, int);
    ~Light();
    void setUsed();
    bool isGreen(int streetId, int time);
    bool canCross(int);
    void addToSchedule(int, int);
    void reset();
    Light *copy();
};
