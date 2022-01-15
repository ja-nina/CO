#pragma once

#include <unordered_set>

#include <utility>
#include <ctime>
#include "problem.h"
#include "solution.h"
#include "light.h"

class Solver {
public:
    float temperature = 1.0;
    std::unordered_set<int> usedStreets;
    //list of problematic cloggy streets - this will be used for mutation - the more clogged a street, or at least if it is above average the more of a chance for mutation= used for CHANGING TIME
    std::map<double, int> ProblematicStreets;
    //list of problematic cloggy lights - this will be used for mutation - the more clogged a street, or at least if it is above average the more of a chance for mutation = used for changing KOLEJNOŚĆ
    std::map<double, int> ProblematicLights;
    std::vector<std::pair<double, Solution *>> pool;
    std::unordered_set<int> TabuSet; // żeby sb dupy nie zawracać sprawdzaniem znowu tego samego - long bo bd tylko hash sprawdzać
    Problem *problem;
    Solution *mutate(Solution *solution, int seed, bool ifChangeOrder, bool ifDelete, bool ifChangeLen);
    static std::vector<Solution *> crossover(Solution *parent1, Solution *parent2);
    void createPool(Solution *BaseSolution);
    void setProblem(Problem *problem);
    void getBestFit(Solution *solution);
    static Solver &getInstance() {
        static Solver instance;
        return instance;
    }
    Solution* go();
    Solution *dropThosePeskyCars(Solution *solution, float thereshold);
};
