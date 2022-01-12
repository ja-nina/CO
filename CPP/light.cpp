#include "light.h"


Light::Light(int id, int T){
    this->id = id;
    this->T = T;
    this->differenceInCycle = 0;

}

void Light::setUsed(){
    for (int i=0;i<T;i++) this->used.push_back('0'); //chodzi o to, że vector<bool> jest zjebany a int zajmuje więcej miejsca niz bool
}


Light::~Light(){
    used.clear();

}
void Light::addStreet(int idStreet){
    this->schedule[idStreet] = 1;
    this->starting[idStreet] =this->cycle;
    this->cycle += 1;
}

bool Light::isGreen(int idStreet, int time){
    //std::cout<<"isgreen? light:" <<id<<" str: "<<idStreet<<std::endl;
    //for(auto& item: starting){
    //    std::cout<<"  "<< item.first<<" - "<< item.second<<std::endl;
    //}
     if(this->starting.count(idStreet) == 1 && this->schedule.count(idStreet) == 1){
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
    if(length == 0){ 
        this->starting[idStreet] = -1; 
        return;
    }
    //std::cout<<"lol1"<<std::endl;
    this->starting[idStreet] = this->cycle;
    this->usedStreets.push_back(idStreet);
    this->numberOfUsedStreets += 1;
    this->cycle += length;
    //std::cout<<"lol2"<<std::endl;
}

void Light::reset(){
    for(int i = 0; i< this->T; i++){
        this->used[i] = '0';
    }
    this->differenceInCycle = 0;
}

Light* Light::copy(){
    Light* newLight = new Light(this->id, this->T);
    for (auto& it: this->schedule) {
        newLight->schedule[it.first] = it.second;}
    for (auto& it: this->starting) {
        newLight->starting[it.first] = it.second;}
    newLight->cycle = cycle;
    //newLight->usedStreets = this->usedStreets;
    newLight->numberOfUsedStreets = this->numberOfUsedStreets;
    //newLight->used = this->used;
    for(auto element: this->used){
        newLight->used.push_back(element);
    }
    for(auto element: this->usedStreets){
        newLight->usedStreets.push_back(element);
    }
return newLight;
}

