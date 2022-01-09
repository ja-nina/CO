#include "solver.h"
#include <math.h>
#include <map>
#include <chrono>

//maximal cloggyness

//population pool
void Solver::createPool(Solution* BaseSolution){
    srand(time(NULL));
    int i = 0; int score;
    int baseScore = problem->simulate();
    
    std::cout<<"base score: "<< baseScore<<std::endl;
    while(i<1){
        std::cout<<"START"<<std::endl;
        Solution* solution;
        std::cout<<"RESET"<<std::endl;
        
        std::cout<<"MUTATION"<<std::endl;
        solution = this->mutate(BaseSolution, floor(rand()* 100000), false, false);
        problem->Lights = solution->Lights;
        problem->reset();
        score = problem->simulate();
        std::cout<<i<<" score :"<< score<<" approxore: "<< problem->approxsimulate()<<" base score: "<< baseScore<<std::endl;
        if(score > baseScore){
            i++;
            pool.insert({score, solution});
            std::cout<<i<<" another down "<<std::endl;
        }else{
            std::cout<<"deleting"<<std::endl;
            delete solution;
            
        }
    }
    
    for(auto& element: pool){
        std::cout<<element.first<<" " << (long) element.second->getHash()<<std::endl;
    }
    std::cout<<"END OF POOL CREATION"<<std::endl;
}

//getBest fit

//kinda hillclimby
//mutation
Solution* Solver::mutate(Solution* solution, int  seed, bool ifDelete, bool ifChangeLen){
    paramlights = (float) ProblematicLights.size()/ (float) problem->lights;
    paramstreets = (float) ProblematicStreets.size()/ (float) problem->streets; // wsm niepotrzebne ale może słyżyć do wzynaczani i i j więc zostawiam (tyle ile ma być zmienianych /mutowanych świateł)
    int index; int swap2; int swap1; int temp; int przesuniecie; int start1; int start2;
    Solution* newSolution = new Solution(solution->Lights);
    int i = 0; int imax = floor((temperature*0.5 + 0.5) *  ProblematicLights.size()) * (float) (rand()%50 + 25) / (float) 100;
    //jeśli chodzi o te wzory to tam mniejsza to jest raczej hyperparametr
    std::map<double, int>::reverse_iterator it;
    srand(seed);
    
    for (it = ProblematicLights.rbegin(); it != ProblematicLights.rend(); ++it){
        index = it->second;
        if( rand()%100 > 50 && newSolution->Lights[index]->numberOfUsedStreets > 1){ // żeby było trochę bardziej stochastycznie + wiadomo nie mutować ulic tylko z 1 used str
            
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
    int start; 

    // możliwośc deletowania ulic danych 
    if(ifDelete == true){
     int indexdelete;int difference;
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
     int jmax = floor(temperature *   (float) (rand()% 5 + 1)/ (float) 100 *  ProblematicStreets.size());
     int j = 0;int currentStreet; int indxLight;
     for (it = ProblematicStreets.rbegin(); it != ProblematicStreets.rend(); ++it){
         //if( rand() > paramstreets * temperature){
         if( (float) (rand()%100) > 90){
             j+=1;
             //std::cout<<"j: "<<j<<" jmax: "<< jmax<<std::endl;
             currentStreet = it->second;
             indxLight = this->problem->Streets[currentStreet]->end;
             Light* newLight = solution->Lights[indxLight]->copy();
             //std::cout<< indxLight<<std::endl;
             newLight->schedule[currentStreet] +=1;
             start = newLight->starting[currentStreet];  
             newLight->cycle +=1;

             for (auto& it2: newLight->starting) {
                 if(it2.second > start){newLight->starting[it2.first]  = it2.second + 1;}
             }

             //bring in feature that decreases length

         newSolution->Lights.at(indxLight) = (Light*) newLight;
         }
         if(j > jmax){
             //std::cout<<" j: "<<j<<std::endl;
             break;}
         }
     }
        

        long hash = newSolution->getHash();
        if (TabuSet.find(hash) != TabuSet.end()) {
            std::cout<<"FOUND!"<<std::endl;
            delete newSolution;
            return mutate(solution, seed + 13, false, false);
        }
        TabuSet.insert(newSolution->getHash());
        return newSolution;
    }

//cross

Solution& Solver::getBestFit(){}

void Solver::setProblem(Problem* problem){
            this->problem = problem;
            this->streets = problem->streets;
            this->lights = problem->lights;
            }


void Solver::go(){
    srand(time(0));
    Solution* solution;
    std::set<std::pair<double, Solution*>> newPool;
    int j = 0; int score;
    bool ifDelete; bool ifChangeLen;
    int streakFail = 0;
    std::cout<<"lights: "<<problem->lights<<" streets: "<< problem->streets<< " problematic lights: "<< ProblematicLights.size() <<  " problematic streets: "<< ProblematicStreets.size() << std::endl;
    while(j <100){
        for(auto& pair: pool){
        while(true){
            //std::cout<<"START"<<std::endl;
            ifDelete = (rand()%4== 1 && j > 10) || ( rand()%2 == 1 && streakFail >  10);
            ifChangeLen = rand()%3 ==2 || ( rand()%2 == 1 && streakFail >  5);
            //std::cout<<"MUTATE"<<std::endl;
            solution = this->mutate(pair.second, floor(rand()* 100000), ifDelete, ifChangeLen);
            
            //std::cout<<"ASSIGN"<<std::endl;
            problem->Lights = solution->Lights;
            //std::cout<<"RESET"<<std::endl;
            problem->reset();
            score = problem->simulate();
            std::cout<<j<<" del: "<<ifDelete<<" lenchange: "<< ifChangeLen<< " score :"<< score <<" approxore: "<< problem->approxsimulate()<<" base score: "<< pair.first<<std::endl;
            if(score > pair.first){
                newPool.insert({score, solution});
                //std::cout<<j<<" another down "<<std::endl;
                
             break;
            }else{
                delete solution;
                //-delete those new lights
            }
            streakFail +=1;
            }
        j++;
        streakFail == 0;

        
    }
    for(auto& element: pool){
        std::cout<<j<<" "<<element.first<<" " << (long) element.second->getHash()<<std::endl;
    }
    pool.clear();
    pool = newPool;
    newPool.clear();
}
}
