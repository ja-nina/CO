#pragma once
#include <vector>
#include <unordered_map>
#include <set>
#include <map>

class Light{
    public:
    Light(int, int); //id, time
    ~Light(){};
    //std::unordered_map<int,int> order;
    std::unordered_map<int, int> starting;
    std::unordered_map<int,int> schedule;
    std::vector<int> usedStreets; //different from used!
    int numberOfUsedStreets = 0;
    int id; 
    int usage = 1;
    int T;
    int cycle = 0;
    std::vector<char> used;

    int differenceInCycle;
    //std::set <int> streets;
    void setUsed();
    void addStreet(int);
    bool isGreen(int streetId, int time);
    bool canCross(int);
    void addToSchedule(int, int);
    void reset();
    Light* copy();
};
