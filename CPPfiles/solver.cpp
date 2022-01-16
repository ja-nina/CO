#include <cmath>
#include <map>
#include <chrono>
#include "solver.h"

void Solver::setProblem(Problem *problem) {
    this->problem = problem;
}


void Solver::createPool(Solution *BaseSolution) {
    srand(time(nullptr));
    int i = 0;
    int score;
    while (i < 30) {
        Solution *solution;
        solution = this->mutate(BaseSolution, floor(rand() * 100000), 0, true, false, false);
        problem->Lights = solution->Lights;
        score = problem->simulate();
        problem->reset();
        pool.emplace_back(score, solution);
        i++;
    }
}

Solution *
Solver::mutate(Solution *solution, int seed, int counter, bool ifChangeOrder, bool ifDelete, bool ifChangeLen) {
    int index, swap2, swap1, temp, przesuniecie, start1, start2;
    auto *newSolution = new Solution(solution->Lights);
    int i = 0;
    int imax = floor((temperature * 0.5 + 0.5) * ProblematicLights.size()) * (float) (rand() % 50 + 25) / (float) 100;
    std::map<double, int>::reverse_iterator it;
    srand(seed);
    if (ifChangeOrder) {
        for (it = ProblematicLights.rbegin(); it != ProblematicLights.rend(); ++it) {
            index = it->second;
            if (rand() % 100 > 60 && newSolution->Lights[index]->numberOfUsedStreets >
                                     1) {
                Light *newLight = solution->Lights[index]->copy();
                i += 1;
                for (int r = 0; r < 0.3 * ((float) (rand() % 100) / (float) 100) * temperature *
                                    newSolution->Lights[index]->numberOfUsedStreets; r++) {

                    swap1 = newLight->usedStreets[rand() % newLight->usedStreets.size()];
                    swap2 = swap1;
                    while (swap2 == swap1) {
                        swap2 = newLight->usedStreets[rand() % newLight->usedStreets.size()];
                    }
                    start1 = newLight->starting[swap1];
                    start2 = newLight->starting[swap2];
                    if (start2 < start1) {
                        temp = swap1;
                        swap1 = swap2;
                        swap2 = temp;
                        temp = start2;
                        start2 = start1;
                        start1 = temp;
                    }
                    przesuniecie = newLight->schedule[swap2] - newLight->schedule[swap1];
                    newLight->starting[swap1] = start2 + przesuniecie;
                    newLight->starting[swap2] = start1;
                    for (auto &it3: newLight->starting) {
                        if (it3.second > start1 && it3.second < start2) {
                            newLight->starting[it3.first] = it3.second + przesuniecie;
                        }
                    }
                }
                newSolution->Lights.at(index) = (Light *) newLight;
            }
            if (i > imax) { break; }
        }
    }
    int start;
    if (ifDelete) {
        int indexdelete, difference;
        for (int i = 0; i < 0.01 * problem->streets; i++) {
            indexdelete = rand() % problem->streets;
            if (problem->Streets[indexdelete]->entered == 0 && problem->Streets[indexdelete]->passed == 0) {
                if (solution->Lights[problem->Streets[indexdelete]->end]->cycle == 0) {
                    continue;
                }
                Light *newLight = solution->Lights[problem->Streets[indexdelete]->end]->copy();
                difference = newLight->schedule[indexdelete];
                start = newLight->starting[indexdelete];
                newLight->schedule[indexdelete] = 0;
                newLight->starting[indexdelete] = -1;
                for (auto &it2: newLight->starting) {
                    if (it2.second > start) {
                        newLight->starting[it2.first] = it2.second - difference;
                    }
                }
            }
        }
    }
    if (ifChangeLen) {
        int lol = 0;
        int currentStreet, indexMaxi;
        float maxi, mean, pom;
        for (Light *light: solution->Lights) {
            if (rand() % 3 == 1) {
                mean = 0;
                maxi = 0;
                for (int streetId: light->usedStreets) {
                    pom = problem->Streets[streetId]->waitingTime / light->schedule[streetId];
                    if (pom > maxi) {
                        indexMaxi = streetId;
                        maxi = pom;
                    }
                    mean += pom;
                }
                if (maxi > 2 * (mean / std::max(1, (light->numberOfUsedStreets)))) {
                    currentStreet = indexMaxi;
                    Light *newLight = light->copy();
                    newLight->schedule[currentStreet] += 1;
                    start = newLight->starting[currentStreet];
                    newLight->cycle += 1;
                    lol += 1;
                    for (auto &it2: newLight->starting) {
                        if (it2.second > start) { newLight->starting[it2.first] = it2.second + 1; }
                    }
                    newSolution->Lights.at(light->id) = (Light *) newLight;
                }
            }
        }
    }
    long hash = newSolution->getHash();
    if (TabuSet.find(hash) != TabuSet.end()) {
        if (counter == 5000) {
            return newSolution;
        }
        delete newSolution;
        return mutate(solution, seed + 13, counter + 1, true, false, false);
    }
    TabuSet.insert(newSolution->getHash());
    return newSolution;
}

std::vector<Solution *> Solver::crossover(Solution *parent1, Solution *parent2) {
    std::vector<Solution *> children;
    auto child1 = new Solution(parent1->Lights);
    auto child2 = new Solution(parent2->Lights);
    for (int i = 0; i < (int)   parent1->Lights.size(); i++) {
        float r = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
        if (r < 0.5) {
            child1->Lights[i] = parent2->Lights[i];
            child2->Lights[i] = parent1->Lights[i];
        }
    }
    children.push_back(child1);
    children.push_back(child2);
    return children;
}

Solution *Solver::go(std::chrono::high_resolution_clock::time_point start) {
    srand(time(nullptr));
    problem->reset();
    Solution *parent1;
    Solution *parent2;
    std::vector<std::pair<double, Solution *>> newPool;
    std::vector<std::pair<double, Solution *>> tournament;
    int noImprov = 0;
    double bestScore;
    int score1, score2, c;
    bool ifDelete, ifChangeLen, ifChangeOrder;
    bool foundBetter = false;
    std::sort(pool.begin(), pool.end(), [](auto &left, auto &right) {
        return left.first > right.first;
    });
    bestScore = pool[0].first;
    while (noImprov < 50) {
        auto stop = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::seconds>(stop - start);
        if (duration.count() > 270) {
            Solution *bestSol = pool[0].second;
            return bestSol;
        }
        newPool.emplace_back(pool[0].first, pool[0].second);
        newPool.emplace_back(pool[1].first, pool[1].second);
        for (int i = 0; i < (int) pool.size() / 2 - 1; i += 1) {
            random_unique(pool.begin(), pool.end(), 6);
            c = 0;
            for (auto &pair: pool) {
                if (c < 6)
                    tournament.emplace_back(pair);
                else
                    break;
                c++;
            }
            std::sort(tournament.begin(), tournament.end(), [](auto &left, auto &right) {
                return left.first > right.first;
            });
            parent1 = tournament[0].second;
            parent2 = tournament[1].second;
            tournament.clear();
            ifDelete = rand() % 3 == 1;
            ifChangeLen = rand() % 3 == 2;
            ifChangeOrder = rand() % 3 == 0;
            std::vector<Solution *> children = this->crossover(parent1, parent2);
            Solution *child1 = children[0];
            Solution *child2 = children[1];
            float r = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
            if (r < 0.35)
                child1 = this->mutate(child1, floor(rand() * 100000), 0, ifChangeOrder, ifDelete, ifChangeLen);
            r = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
            if (r < 0.35)
                child2 = this->mutate(child2, floor(rand() * 100000), 0, ifChangeOrder, ifDelete, ifChangeLen);
            problem->Lights = child1->Lights;
            score1 = problem->simulate();
            if (score1 > bestScore) {
                foundBetter = true;
                bestScore = score1;
            }
            problem->reset();
            problem->Lights = child2->Lights;
            score2 = problem->simulate();
            if (score2 > bestScore) {
                foundBetter = true;
                bestScore = score2;
            }
            problem->reset();
            newPool.emplace_back(score1, child1);
            newPool.emplace_back(score2, child2);
        }
        noImprov++;
        if (foundBetter)
            noImprov = 0;
        pool.clear();
        pool = newPool;
        std::sort(pool.begin(), pool.end(), [](auto &left, auto &right) {
            return left.first > right.first;
        });
        newPool.clear();
    }
    Solution *bestSol = pool[0].second;
    return bestSol;
}
