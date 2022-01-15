#include "problem.h"
#include "solver.h"
#include <cmath>
#include <unordered_set>
#include <iostream>
#include <fstream>
#include <cmath>
#include <algorithm>

Problem::Problem(int T, int numLights, int numStreets, int numCars, int reward) {
    this->T = T;
    this->lights = numLights;
    this->streets = numStreets;
    this->cars = numCars;
    this->R = reward;
    this->cloggyness = new float[numStreets];
    this->cloggynessLights = new float[numLights];
    for (int id = 0; id < this->streets; ++id) {
        *(this->cloggyness + id) = 0;
    }
    for (int id = 0; id < this->lights; ++id) {
        *(this->cloggynessLights + id) = 0;
    }
}

void Problem::_situateCars() {
    for (Car *car: this->Cars) {
        car->number = this->Streets[car->road[0]]->carEnter();
        car->seq = 0;
        Streets[car->road[0]]->history[0] = 1;
    }
}

int Problem::carCrosses(int carId, int currentStreet, int currentLight, int second) {
    this->Streets[currentStreet]->carLeave();
    this->Lights[currentLight]->used[second] = '1';
    int nextStreet = this->Cars[carId]->cross(second);
    if (!this->Cars[carId]->ostatniaProsta()) {
        this->Cars[carId]->number = this->Streets[nextStreet]->carEnter();
    }
    Streets[nextStreet]->history.insert({second + Streets[nextStreet]->length, 1});
    return nextStreet;
}

int Problem::simulate() {
    this->_situateCars();
    int profit = 0;
    int currentStreet;
    int nextLight;
    bool isGreen;
    bool isFirst;
    bool isTransit;
    bool ended[this->cars] = {false};
    for (int second = 0; second < this->T; second++) {
        for (Car *car: this->Cars) {
            if (ended[car->id]) {
                continue;
            }
            currentStreet = car->road[car->seq];
            nextLight = this->Streets[currentStreet]->end;
            isGreen = this->Lights[nextLight]->isGreen(currentStreet, second);
            if (car->ostatniaProsta()) {
                int bonus = this->T - this->Streets[currentStreet]->length - car->lastCross;
                if (bonus >= 0) {
                    profit += this->R + bonus;
                }
                ended[car->id] = true;
                continue;
            }
            if (!this->Lights[nextLight]->canCross(second)) {
                if (!car->isInTransit(second, this->Streets[currentStreet]->length)) {
                    this->Streets[currentStreet]->waitingTime += car->priority;
                }
                continue;
            }
            isFirst = car->number == this->Streets[currentStreet]->passed;
            isTransit = car->isInTransit(second, this->Streets[currentStreet]->length);
            if (isGreen && isFirst && !isTransit) {
                this->carCrosses(car->id, currentStreet, nextLight, second);
            } else {
                this->Streets[currentStreet]->waitingTime += car->priority;
            }
        }
    }
    //part for updating waiting time
    for (Street *street: Streets) {
        Solver::getInstance().ProblematicStreets[street->waitingTime] = street->id;
    }
    return profit;

}

Solution *Problem::preprocessor() {
    //first calculating LB
    int tempNumberOfUsedStreets[this->lights] = {0};
    float temporary;
    float meanLB = 0;
    float tempdenom = 0;
    for (Car *car: this->Cars) {
        for (int street: car->road) {
            if (street == car->road[0]) { continue; }
            car->LB += this->Streets[street]->length;
            *(cloggyness + street) = *(cloggyness + street) + 1;
        }
        if (car->LB - 1 <= this->T) {
            int num = 0;
            tempdenom += 1;
            meanLB += car->LB;
            for (int street: car->road) {
                num += 1;
                Streets[street]->situation += (float) num * car->LB / (float) car->length;
                Streets[street]->useness += 1;
                Solver::getInstance().usedStreets.insert(street);
                tempNumberOfUsedStreets[this->Streets[street]->end] += 1;
            }
            this->Cars[car->id]->priority = Problem::getSigmoidPriority((float) car->LB / (meanLB / tempdenom));
        }
    }

    for (Street *street: Streets) {
        meanstreetSituation += (float) street->situation / (float) street->useness;
    }

    for (int id = 0; id < this->streets; id++) {
        *(this->cloggyness + id) = 0.1 * std::max(*(this->cloggyness + id) / this->T, (float) 1.0) *
                                   std::max(0.1, log(std::max(0, tempNumberOfUsedStreets[this->Streets[id]->end] - 1)));
        *(this->cloggynessLights + this->Streets[id]->end) += *(this->cloggyness + id);
    }

    for (int lightId = 0; lightId < this->lights; lightId++) {
        temporary = *(this->cloggynessLights + lightId);
        if (temporary > 0.1 && tempNumberOfUsedStreets[lightId] > 1) {
            Solver::getInstance().ProblematicLights[(double) temporary + (double) lightId / (double) 1000000] = lightId;
        }
    }

    for (int streetId = 0; streetId < this->streets; streetId++) {
        temporary = *(this->cloggyness + streetId);
        if (temporary > 0.3) {
            Solver::getInstance().ProblematicStreets[(double) temporary +
                                                     (double) streetId / (double) 1000000] = streetId;
        }
    }

    for (const auto &usedStrInx: Solver::getInstance().usedStreets) {
        this->Lights[this->Streets[usedStrInx]->end]->addToSchedule(usedStrInx, 1);
    }
    return new Solution(Lights);
}

void Problem::toAnswer() {
    int numInter = this->Lights.size();
    std::cout << numInter << std::endl;
    for (int i = 0; i < numInter; i++) {
        std::cout << i << std::endl;
        std::unordered_map<int, int> schedule = this->Lights[i]->schedule;
        for (auto& it: schedule) {
            std::cout << this->IdToName[it.first] << " " << it.second << std::endl;
        }
    }
}

int Problem::approxsimulate() {
    this->_situateCars();
    int profit = 0;
    int nextLight;
    int bonus;
    bool ended[this->cars] = {false};
    for (Car *car: this->Cars) {
        if (car->LB < this->T) {
            float second = 0;
            for (int streetId: car->road) {
                nextLight = this->Streets[streetId]->end;
                second += (float) *(cloggyness + streetId);
                while (not this->Lights[nextLight]->isGreen(streetId, (int) second) &&
                       streetId != car->road[car->length - 1] && second < this->T) { //while not green
                    second += 1.0;
                }
                second += this->Streets[streetId]->length;
            }
            bonus = this->T - second;
            if (bonus >= 0) {
                profit += this->R + bonus;
            }
        }
    }
    return profit;
}

void Problem::reset() {
    for (Light *light : this->Lights) {
        light->reset();
    }

    for (Car *car : this->Cars) {
        car->reset();
    }

    for (Street *street : this->Streets) {
        street->reset();
    }
}

//change of lengths proportional to useness

float Problem::getSigmoidPriority(float ratioToMean) {
    return 1 / (1 + std::exp(-4 + ratioToMean * 3));
}


