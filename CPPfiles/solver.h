#pragma once

#include <unordered_set>

#include <utility>
#include <ctime>
#include <algorithm>
#include <chrono>
#include "problem.h"
#include "solution.h"
#include "light.h"

class Solver {
public:
    float temperature = 1.0;
    std::unordered_set<int> usedStreets;
    std::map<double, int> ProblematicStreets;
    std::map<double, int> ProblematicLights;
    std::vector<std::pair<double, Solution *>> pool;
    std::unordered_set<int> TabuSet;
    Problem *problem;
    Solution *mutate(Solution *solution, int seed, int counter, bool ifChangeOrder, bool ifDelete, bool ifChangeLen);
    static std::vector<Solution *> crossover(Solution *parent1, Solution *parent2);
    void createPool(Solution *BaseSolution);
    void setProblem(Problem *problem);
    static Solver &getInstance() {
        static Solver instance;
        return instance;
    }
    Solution* go(std::chrono::high_resolution_clock::time_point start);

    template<class bidiiter>
    bidiiter random_unique(bidiiter begin, bidiiter end, size_t num_random) {
        size_t left = std::distance(begin, end);
        while (num_random--) {
            bidiiter r = begin;
            std::advance(r, rand()%left);
            std::swap(*begin, *r);
            ++begin;
            --left;
        }
        return begin;
    }
};
