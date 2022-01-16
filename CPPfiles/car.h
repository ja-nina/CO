#pragma once

#include <vector>

class Car {
public:
    int lastCross = -1;
    std::vector<int> road;
    int LB = 0;
    int length, id, seq, number;
    Car(int, int);
    bool isInTransit(int, int) const;
    int cross(int);
    bool ostatniaProsta() const;
    void reset();
    float priority = 0;
};