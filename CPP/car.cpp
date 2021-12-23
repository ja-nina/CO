#include "car.h"

Car::Car(int id, int length){
    this->id = id;
    this->length = length;

}

bool Car::ostatniaProsta(){
    return (this->seq == (this->length - 1));
}

bool Car::isInTransit(int second, int lengthOfCurrentSt){
    if( this->lastCross == -1){ return false;}
    else{
    return ((second - this->lastCross) < lengthOfCurrentSt);
    }
}

int Car::cross(int time){
    this->seq += 1;
    this->lastCross = time;
    return this->road[this->seq];
}