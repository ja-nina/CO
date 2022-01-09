#include "problem.h"
#include "solver.h"
#include <unordered_set>
#include <iostream>
#include <fstream>
#include <math.h>
#include <algorithm>

    Problem::Problem(int T, int numLights, int numStreets, int numCars, int reward){
        this->T = T;
        this->lights = numLights;
        this->streets = numStreets;
        this->cars = numCars;
        this->R = reward;
        this->cloggyness = new float[numStreets];
        this->cloggynessLights = new float[numLights];
        for (int id = 0; id < this->streets; ++id){
            *(this->cloggyness + id) = 0;
        }

        for (int id = 0; id < this->lights; ++id){
            *(this->cloggynessLights + id) = 0;
        }
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
            //std::cout<<"SIMULATION MIDDLE" <<std::endl;
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
                //std::cout<<second<< " "<< car->id<<" "<<isGreen << " "<<isFirst << " " << !isTransit<<std::endl;
                if(isGreen && isFirst && ! isTransit){
                       this->carCrosses(car->id, currentStreet, nextLight, second);

                }
            }
        }
        return profit;

    }

    Solution* Problem::preprocessor(){
        //first calculating LB
        int tempNumberOfUsedStreets[this->lights] = {0};
        float temporary;

        for(Car* car: this->Cars){
            for(int street: car->road){
                car->LB += this->Streets[street]->length;
                *(cloggyness + street) = *(cloggyness + street) + 1;
            
            }
            if(car->LB <= this->T){
                for(int street: car->road){
                    Solver::getInstance().usedStreets.insert(street);
                    tempNumberOfUsedStreets[this->Streets[street]->end] +=1;
                    
            }
            }
        }
        for (int id = 0; id < this->streets; id++){
            *(this->cloggyness + id) = 0.1*std::max(*(this->cloggyness + id)/this->T, (float)1.0) *std::max(0.1, log( std::max(0,tempNumberOfUsedStreets[this->Streets[id]->end] - 1)));
            *(this->cloggynessLights + this->Streets[id]->end) += *(this->cloggyness + id);
            
        }
        for(int lightId= 0 ; lightId < this->lights; lightId ++){
            temporary = *(this->cloggynessLights + lightId);
            if( temporary > 0.1 && tempNumberOfUsedStreets[lightId] > 1){
                Solver::getInstance().ProblematicLights[(double) temporary + (double) lightId/(double)1000000] = lightId;
            }}

        for(int streetId= 0 ; streetId < this->streets; streetId ++){
            temporary = *(this->cloggyness + streetId);
            if( temporary > 0.3){
                Solver::getInstance().ProblematicStreets[(double) temporary + (double) streetId/(double)1000000] = streetId;
            }}

        //only adding used streets 
        for (const auto& usedStrInx: Solver::getInstance().usedStreets) {
           this->Lights[this->Streets[usedStrInx]->end]->addToSchedule(usedStrInx, 1);
           //this->Lights[this->Streets[usedStrInx]->end]->numberOfUsedStreets +=1;
           }

        //set solver stuff
        //Solver::getInstance().setProblem(this);
        return new Solution(Lights);}

    int Problem::approxsimulate(){

        this->_situateCars();
        int profit = 0; int nextLight; int bonus;
        // ended might be implemented as a vector 
        bool ended[this->cars] = {false};
        for( Car* car: this->Cars){
            if(car->LB < this->T){
                float second = 0;
                for(int streetId: car->road){

                    nextLight = this->Streets[streetId]->end;
                    second += (float) *(cloggyness + streetId); 
                    while(not this->Lights[nextLight]->isGreen(streetId, (int) second) && streetId != car->road[car->length - 1] && second < this->T){ //while not green
                        second += 1.0;
                    }
                    second += this->Streets[streetId]->length;
                }
                bonus =  this->T - second;
                if(bonus >= 0 ){
                    profit += this->R + bonus;
                }
        }}
    return profit;
    }

    void Problem::reset(){
        // to się da ulepszyć, ale to tam mniejsza
        for( Light* light : this->Lights){
            light->reset();
        }

        for( Car* car : this->Cars){
            car->reset();
        }

        for( Street* street : this->Streets){
            street->reset();
        }

    }

