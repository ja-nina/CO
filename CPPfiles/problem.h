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
    float *cloggyness;
    float *cloggynessLights;
    std::vector<Light *> Lights;
    std::vector<Car *> Cars;
    std::vector<Street *> Streets;
    std::unordered_map<int, std::tuple<int, int, int>> runningCars;
    std::unordered_map<std::string, int> NameToId;
    std::unordered_map<int, std::string> IdToName;
    void toAnswer();
    Problem(int, int, int, int, int);
    int simulate();
    Solution *preprocessor();
    void reset();
    void _situateCars();
    int carCrosses(int, int, int, int);
    float meanstreetSituation = 0;
    static float getSigmoidPriority(float ratioToMean);
};