#include "problem.h"
#include <unordered_set>
#include <iostream>
#include <fstream>


    Problem::Problem(int T, int numLights, int numStreets, int numCars, int reward){
        this->T = T;
        this->lights = numLights;
        this->streets = numStreets;
        this->cars = numCars;
        this->R = reward;
        
    }

    void Problem::_situateCars(){
        for( Car* car: this->Cars){
            //int first_str = car->road[0];
            car->number = this->Streets[car->road[0]]->carEnter();
            car->seq = 0;
        }
    }
    
    int Problem::carCrosses(int carId, int currentStreet, int currentLight, int second){
        this->Streets[currentStreet]->carLeave();
        this->Lights[currentLight]->used[second] = '1';
        int nextStreet = this->Cars[carId]->cross(second);
        if (! this->Cars[carId]->ostatniaProsta()){
            this->Cars[carId]->number = this->Streets[nextStreet]->carEnter();
        }
        return nextStreet;
    }
    //log 

    int Problem::simulate(){

        this->_situateCars();
        int profit = 0; int currentStreet; int nextLight;
        bool isGreen;
        bool isFirst;
        bool isTransit;
        // ended might be implemented as a vector 
        bool ended[this->cars] = {false};
        //par where 
        for(int second= 0; second < this->T; second++){
            for( Car* car: this->Cars){
                if(ended[car->id]){
                    continue;
                }
                //implement privilege 
                //implement weights
                currentStreet = car->road[car->seq];
                nextLight = this->Streets[currentStreet]->end;
                isGreen = this->Lights[nextLight]->isGreen(currentStreet, second);
                if(car->ostatniaProsta()){
                    int bonus = this->T - this->Streets[currentStreet]->length - car->lastCross;
                    if(bonus >= 0){
                        profit += this->R + bonus;
                    }
                    ended[car->id] = true;
                    continue;
                }

                if(this->Lights[nextLight]->canCross(second) == false){
                    continue;
                }
                isFirst = car->number == this->Streets[currentStreet]->passed;
                isTransit = car->isInTransit(second, this->Streets[currentStreet]->length);
                if(isGreen && isFirst && ! isTransit){
                        // to be implemented
                        //self.arrival_at_end[current_street].append((second, weight)) # time, weight /// this will be cared for later
                        //self.Lights[next_light].usedStreets.add(current_street)// this will be cared for later
                        this->carCrosses(car->id, currentStreet, nextLight, second);

                }
            }
        }
        return profit;

    }

    void Problem::preprocessor(){
        //first calculating LB
        std::unordered_set<int> usedStreets;
        for(Car* car: this->Cars){
            for(int street: car->road){
                car->LB += this->Streets[street]->length;
            }
            if(car->LB <= this->T){
                for(int street: car->road){
                    usedStreets.insert(street);
            }
            }
        }
        // * COMMENTED OUT FOR TEST *
        // for (auto id = usedStreets.begin(); id != usedStreets.end(); ++id) {
        //     this->Lights[this->Streets[*id]->end]->addStreet(*id);
        //     }
        
        //   for (Light* light : this->Lights){
        //       std::cout<<" ID of light:  "<< light->id<<std::endl;
        //           for (auto const &pair:light->starting) {
        //           std::cout << "{" << pair.first << ": " << pair.second << "}\n";
        //       }
        //           for (auto const &pair:light->schedule) {
        //           std::cout << "{" << pair.first << ": " << pair.second << "}\n";
        //  }}
    }
