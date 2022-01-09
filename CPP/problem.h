#pragma once
#include "light.h"
#include "street.h"
#include "car.h"
#include "solution.h"
#include <unordered_map>

class Problem {
    public:
    int T, R;
    int lights, streets, cars;
    float* cloggyness;
    float* cloggynessLights;
    std::vector<Light*> Lights;
    std::vector<Car*> Cars; // not sure if this is necessary
    std::vector<Street*> Streets;
    std::unordered_map<int,std::tuple<int, int, int>> runningCars; //dic with numer, last_intersection, and number of current street // idk if this is a good idea or not
    // czy bd szybciej czy wolniej z tym czy bez ale no przekonamy sie

    std::unordered_map<std::string, int> NameToId;
    std::unordered_map<int, std::string> IdToName;

    Problem(int, int, int, int, int);
    int simulate();
    int approxsimulate();
    void add_streets();
    void addStreet(Street*);
    void addCar(Car*);
    void addLight(Light*);
    Solution* preprocessor();
    void reset();

    bool isFirst();
    void _situateCars();
    int carCrosses(int, int, int, int);
    
};