#include "light.h"


Light::Light(int id, int T){
    this->id = id;
    this->T = T;
    for (int i=0;i<T;i++) this->used.push_back('0'); //chodzi o to, że vector<bool> jest zjebany a int zajmuje więcej miejsca niz bool
    this->differenceInCycle = 0;

}

void Light::addStreet(int idStreet){
    this->schedule[idStreet] = 1;
    this->starting[idStreet] =this->cycle;
    this->cycle += 1;
}

bool Light::isGreen(int idStreet, int time){
     if(this->starting.count(idStreet)){
         if(this->starting[idStreet] == -1){
             return false;
         }
         if((this->starting[idStreet] <= time % this->cycle) && (time % this->cycle < (this->starting[idStreet] + this->schedule[idStreet]))){
             return true;
         }
     }
    return false;
}

bool Light::canCross(int time){
    bool ans= this->used[time] == '0';
    return ans;
}

void Light::addToSchedule(int idStreet, int length){
    this->schedule[idStreet] = length;
    if(length == 0){ this->starting[idStreet] = -1; return ;}
    this->starting[idStreet] = this->cycle;
    this->cycle += length;
}

void Light::reset(){
    this->used[this->T] = {'0'};
}
