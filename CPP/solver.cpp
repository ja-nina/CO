#include "solver.h"
#include <math.h>
#include <map>
#include <chrono>

//maximal cloggyness

//population pool
void Solver::createPool(Solution* BaseSolution){
    srand(time(NULL));
    int i = 0; int score;
    while(i<30){
        Solution* solution;
        solution = this->mutate(BaseSolution, floor(rand()* 100000), true, false, false);
        problem->Lights = solution->Lights;
        score = problem->simulate();
        problem->reset();
        pool.emplace_back(score, solution);
        i++;
    }
}

//getBest fit

//kinda hillclimby
//mutation
Solution* Solver::mutate(Solution* solution, int  seed, bool ifChangeOrder, bool ifDelete, bool ifChangeLen){
    int index; int swap2; int swap1; int temp; int przesuniecie; int start1; int start2;
    Solution* newSolution = new Solution(solution->Lights);
    int i = 0; int imax = floor((temperature*0.5 + 0.5) *  ProblematicLights.size()) * (float) (rand()%50 + 25) / (float) 100;
    std::map<double, int>::reverse_iterator it;
    srand(seed);
    if(ifChangeOrder){
    for (it = ProblematicLights.rbegin(); it != ProblematicLights.rend(); ++it){ 
        index = it->second;
        if( rand()%100 > 60 && newSolution->Lights[index]->numberOfUsedStreets > 1){ // żeby było trochę bardziej stochastycznie + wiadomo nie mutować ulic tylko z 1 used str
            
            Light* newLight = solution->Lights[index]->copy();
            i +=1;
            for(int r = 0; r< 0.3*((float) (rand()% 100)/(float) 100)*temperature*newSolution->Lights[index]->numberOfUsedStreets; r++){
            // tops 60% mutowana - a jako że połowa *2 = no bo pary to wiadomo 
            swap1 = newLight->usedStreets[rand()% newLight->usedStreets.size()];
            swap2 = swap1; // wybieram 2 ulice co bd zamienione miejscami
    
            while(swap2 == swap1){ 
                swap2 = newLight->usedStreets[rand()% newLight->usedStreets.size()];
            }
            
            start1 = newLight->starting[swap1];
            start2 = newLight->starting[swap2];
            if(start2 < start1){
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
            for (auto& it3: newLight->starting) {
                if(it3.second > start1 && it3.second < start2){
                    newLight->starting[it3.first] = it3.second + przesuniecie;
                    if(it3.first < 0){std::cout<< "ERROR!"<<std::endl;}
                }
                }
            }
            newSolution->Lights.at(index) = (Light*) newLight;
        }
        if(i > imax){break;}
    }
}
    int start; 

    // możliwośc deletowania ulic danych 
    if(ifDelete == true){
     int indexdelete;int difference;
     //build in proclivity towards street at end
     for(int i = 0; i < 0.01*problem->streets; i++){
         indexdelete = rand()%problem->streets;
         if(problem->Streets[indexdelete]->entered == 0 && problem->Streets[indexdelete]->passed == 0){
             if(solution->Lights[problem->Streets[indexdelete]->end]->cycle ==0){continue;}
             std::cout<<"JUST DELETED SMTHNG!";
             Light* newLight = solution->Lights[problem->Streets[indexdelete]->end]->copy();
             difference = newLight->schedule[indexdelete];
             start = newLight->starting[indexdelete];
             newLight->schedule[indexdelete] = 0;
             newLight->starting[indexdelete] = -1;
      
             for (auto& it2: newLight->starting) {
                 if(it2.second > start){newLight->starting[it2.first]  = it2.second - difference;}
             
        }
    }
    }
    }
    if(ifChangeLen == true){
        int lol = 0;
    //int jmax = floor(temperature *   (float) (rand()% 5 + 1)/ (float) 100 *  ProblematicStreets.size());
    int j = 0;int currentStreet; int indxLight;int indexMaxi; float maxi = 0; float mean = 0; float pom;
    for(Light* light: solution->Lights){
        if(rand()%3 == 1){
        mean = 0;
        maxi = 0;
        //we need to check if one waiting time is SIGNIFICANTLY LONGER than the others (like), (compare ratio to length of green light in schedule to waiting time)
        for(int streetId: light->usedStreets){
            //std::cout<<"waiting time: "<<problem->Streets[streetId]->waitingTime<<" light schedule: "<<light->schedule[streetId]<<std::endl;
            pom = problem->Streets[streetId]->waitingTime/ light->schedule[streetId];
            if( pom > maxi){ //needs to be changed if we decide to delete streets bc possible div by 0
                indexMaxi = streetId;
                maxi = pom;
            }
            mean += pom;
            }
        
        //std::cout<<"maxi: "<<maxi<<" mean times 2: "<<2*(mean/light->numberOfUsedStreets) <<std::endl;
        if(maxi > 2*(mean/std::max(1,(light->numberOfUsedStreets)))){
            //std::cout<<"j: "<<j<<" jmax: "<< jmax<<std::endl;
            currentStreet = indexMaxi;
            Light* newLight = light->copy();
             //std::cout<< indxLight<<std::endl;
            newLight->schedule[currentStreet] +=1;
            start = newLight->starting[currentStreet];  
            newLight->cycle +=1;
            //std::cout<<"CHANGED LEN!"<<std::endl;
            lol +=1;
             for (auto& it2: newLight->starting) {
                 if(it2.second > start){newLight->starting[it2.first]  = it2.second + 1;}
             }

             //bring in feature that decreases length

         newSolution->Lights.at(light->id) = (Light*) newLight;
         
         }

     }
    }
    std::cout<<"CHANGED LEN in "<< lol <<std::endl;
    }

        long hash = newSolution->getHash();
        if (TabuSet.find(hash) != TabuSet.end()) {
            std::cout<<"FOUND!"<<std::endl;
            delete newSolution;
            return mutate(solution, seed + 13, true, false, false);
        }
        TabuSet.insert(newSolution->getHash());
        return newSolution;
    }

std::vector<Solution*> Solver::crossover(Solution* parent1, Solution* parent2) {
    std::vector<Solution*> children;
    auto child1 = new Solution(parent1->Lights);
    auto child2 = new Solution(parent2->Lights);
    for (int i = 0; i < parent1->Lights.size(); i++) {
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

void Solver::setProblem(Problem* problem){
            this->problem = problem;
            this->streets = problem->streets;
            this->lights = problem->lights;
            }


void Solver::go(){
    srand(time(0));
    problem->reset();
    Solution* parent1;
    Solution* parent2;
    std::vector<std::pair<double, Solution*>> newPool;
    int j = 0; int score1; int score2;
    bool ifDelete; bool ifChangeLen; bool ifChangeOrder;
    std::sort(pool.begin(), pool.end(), [](auto &left, auto &right) {
        return left.first > right.first;
    });
    while(j <50){
        for (int i = 0; i < pool.size(); i+=2) {
            parent1 = pool[i].second;
            parent2 = pool[i+1].second;
            newPool.emplace_back(pool[i].first, parent1);
            newPool.emplace_back(pool[i+1].first, parent2);
            ifDelete = rand() % 3 == 1;
            ifChangeLen = rand() % 3 == 1;
            ifChangeOrder = rand() % 3 == 1;
            std::vector<Solution *> children = this->crossover(parent1, parent2);
            Solution *child1 = children[0];
            Solution *child2 = children[1];
            child1 = this->mutate(child1, floor(rand() * 100000), ifChangeOrder, ifDelete, ifChangeLen);
            child2 = this->mutate(child2, floor(rand() * 100000), ifChangeOrder, ifDelete, ifChangeLen);
            problem->Lights = child1->Lights;
            score1 = problem->simulate();
            problem->reset();
            problem->Lights = child2->Lights;
            score2 = problem->simulate();
            problem->reset();
            newPool.emplace_back(score1, child1);
            newPool.emplace_back(score2, child2);
        }
        std::sort(newPool.begin(), newPool.end(), [](auto &left, auto &right) {
            return left.first > right.first;
        });
        std::size_t const half_size = newPool.size() / 2;
        std::vector<std::pair<double, Solution*>> best(newPool.begin(), newPool.begin() + half_size);
        std::vector<std::pair<double, Solution*>> worst(newPool.begin() + half_size, newPool.end());
        for(auto& pair : worst){
            delete pair.second;
        }
        pool.clear();
        newPool.clear();
        pool = best;
        std::cout << std::endl << j << " " << pool[0].first << std::endl << std::endl;
        j++;
    }
}


//MUST BE RUN AFTER SIMULATION
Solution* Solver::dropThosePeskyCars(Solution* solution, float thereshold){ // like cars that take up no more than 1.3 of time, can be set to 1.5 and then increased/ decreased, best to start from quite a high number like 2
    std::cout<<"before"<<std::endl;
    //for(Light* light : solution->Lights){
    //        std::cout<<"light: "<< light->id<<" with cycle: "<< light->cycle<<" used str:  "<<light->numberOfUsedStreets<<" "<< light->used.size()<<std::endl;
    //        for(auto& item: light->schedule){
    //            std::cout<<"  schedule: "<< item.first<< " "<< item.second<<std::endl;  
    //        }
    //        for(auto& item: light->starting){
    //            std::cout<<"  starting: "<< item.first<< " "<< item.second<<std::endl;  
    //        }
    //        }
    
    
    
    int meanLB = 0;
    int tempdenom = 0; // number of cars that fit
    usedStreets.clear();
    for(Street* street: problem->Streets){
        problem->Streets[street->id]->useness = 0;
    }
    for(Car* car: problem->Cars){  
        if(car->LB <= problem->T){
            car->LB = 0;
            int num = 0;
            tempdenom +=1;
            //meanLB += car->LB;
            for(int street: car->road){
                if(street == car->road[0] || street == car->road[car->length - 1]){continue;}
                car->LB += problem->Streets[street]->waitingTime / problem->Streets[street]->entered;
                car->LB += problem->Streets[street]->length;
        }
        meanLB += car->LB;
        }}
    for(Car* car: problem->Cars){  
        if(car->LB <= thereshold*problem->T){
            for(int street: car->road){
                problem->Streets[street]->useness += 1;
                this->usedStreets.insert(street);
        }
        }
        else{
            car->priority = 0;
        }
    }
    //std::cout<<"clearing"<<std::endl;
    for(Light* light: solution->Lights){
        //std::cout<<light->id<<std::endl;
        //std::cout<<light->id<<" schedule"<< light->schedule.size()<<" "<< light->starting.size()<<std::endl;
        light->schedule.clear();
        //std::cout<<"clearing starting"<<std::endl;
        light->starting.clear();
        //std::cout<<"clearing cycle"<<std::endl;
        light->cycle = 0;
        //std::cout<<"clearing used streets"<<std::endl;
        light->usedStreets.clear();
        //std::cout<<"clearing num of used streets"<<std::endl;
        light->numberOfUsedStreets = 0;
    }
    std::cout<<"adding"<<std::endl;  int light;
    for (int usedStrInx: this->usedStreets) {  
        light = problem->Streets[usedStrInx]->end;
        //std::cout<<usedStrInx<<" light: "<< problem->Streets[usedStrInx]->end<<std::endl;
        solution->Lights[light]->schedule[usedStrInx] = 1;
        solution->Lights[light]->starting[usedStrInx] = solution->Lights[light]->cycle;
        solution->Lights[light]->usedStreets.push_back(usedStrInx);
        solution->Lights[light]->numberOfUsedStreets += 1;
        solution->Lights[light]->cycle += 1;
           }
    std::cout<<"end- AFTER"<<std::endl;
            //for(Light* light : solution->Lights){
            //    std::cout<<"light: "<< light->id<<" with cycle: "<< light->cycle<<" used str:  "<<light->numberOfUsedStreets<<" "<< light->usedStreets.size()<<"starting len" <<light->starting.size()<<" schedual length "<<light->schedule.size()<<std::endl;
            //    for(auto& item: light->schedule){
            //        std::cout<<"  schedule: "<< item.first<< " "<< item.second<<std::endl;  
            //    }
            //    for(auto& item: light->starting){
            //        std::cout<<"  starting: "<< item.first<< " "<< item.second<<std::endl;  
            //    }
            //}

    return solution;
}
//first the lights have to be passed to the problem
void Solver::getBestFit(Solution* solution){
    int suma = 0;
    for(Light* light: solution->Lights){ 
        //only do it for the first x
        if(light->usedStreets.size() > 1){
        float arrayTemp[light->usedStreets.size()][light->cycle] = {{0}};
        int row = 0;
        for(int streetId: light->usedStreets){
            suma = 0;
            for(auto& item: problem->Streets[streetId]->history){
                //std::cout<<"light: "<< light->id<<" Street: "<<streetId<<" "<<item.first<<" "<<item.second<<std::endl;

                for(int i = 0; i< 5; i++){
                    arrayTemp[row][(item.first + i)% light->cycle] += item.second/(float) (i/2 + 1);
                    suma += item.second/(float) (i/2 + 1);
                }
                for(int t = 1; t< light->schedule[streetId]; t++){
                    int moment = 0;
                    while(moment <light->cycle - light->schedule[streetId] + 1){
                      arrayTemp[row][moment] += arrayTemp[row][moment + t];
                      suma += arrayTemp[row][moment + t];
                      moment +=1;
                    }

                }
            }
            for(int t1 = 0; t1< light->cycle; t1++){
                arrayTemp[row][t1] = arrayTemp[row][t1] / (float) suma;
                //std::cout<<"bob "<< arrayTemp[row][t] <<std::endl;
            }
            row +=1;
        }
        //get index of max column
        int starting = 0; float maxi;  int indexMaxi;
        std::unordered_set<int> assigned; 
        while(starting < light->cycle){
            indexMaxi = 0;maxi = 0;
            for(int row = 0; row< light->usedStreets.size(); row++){
            if(arrayTemp[row][starting] > maxi && assigned.find(row) == assigned.end()){
                maxi = arrayTemp[row][starting];
                indexMaxi = row;
            }}
            light->starting[light->usedStreets[indexMaxi]] = starting;
            assigned.insert(indexMaxi);
            starting += light->schedule[light->usedStreets[indexMaxi]];
    }

     //std::cout<<"LIGHT "<<light->id<<" dimensions of the array: "<< light->usedStreets.size()<<"/"<<light->numberOfUsedStreets <<" "<<light->cycle<<std::endl;
     //for(int row = 0; row< light->usedStreets.size(); row++){
     //    for(int t = 0; t< light->cycle; t++){
     //        std::cout<<t<<": "<<arrayTemp[row][t]<<" ";
     //        }
     //   std::cout<<std::endl;
     //   std::cout<<light->schedule[light->usedStreets[row]]<<" - "<< light->starting[light->usedStreets[row]]<<std::endl;
     //}
    }
}
}
