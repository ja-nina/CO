#pragma once

#include <vector>

class Car {
public:
    int lastCross = -1;
    std::vector<int> road;
    int LB = 0;
    int length, id, seq, number;// number of street (in sequence) on which car currenty is (0, 1, 2, 3, 4...)
    //int number; //mighbt be beneficial to not keep it here
    Car(int, int);
    bool isInTransit(int, int) const;
    int cross(int); //returns new road
    bool ostatniaProsta() const;
    void reset();
    float priority = 0;
};