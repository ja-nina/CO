#pragma once
#include <vector>
class Car{
    public:

    int lastCross = -1;
    std::vector <int> road;
    int LB = 0;
    int length; 
    int id;
    int seq; 
    int number;// number of street (in sequence) on which car currenty is (0, 1, 2, 3, 4...)
    //int number; //mighbt be beneficial to not keep it here

    Car(int, int);
    bool isEnd();
    bool isInTransit(int, int);
    int  cross(int); //returns new road
    bool ostatniaProsta();
    void reset();
    float priority = 0;


};


