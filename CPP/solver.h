#pragma once
#include <unordered_set>

#include <utility>
#include <time.h>
#include "problem.h"
#include "solution.h"
#include "light.h"

class Solver{
    private:


    Problem* problem;
    std::set<std::pair<double, Solution*>> pool;
    std::unordered_set<int> TabuSet; // żeby sb dupy nie zawracać sprawdzaniem znowu tego samego - long bo bd tylko hash sprawdzać
    float avgCloggynessStreet;
    float avgCloggynessLight; // the more the intersection is above average the highers the probability of mutation
    float clogStreetSt;
    float clogLightSt;
    float temperature = 1.0;
    float paramlights;
    float paramstreets;


    Solver(){}
    void fillPool();
    
    Solution* cross(Solution* solution1, Solution* solution2);
    //instead of making a constructor
    // SET THE PROBLEM 
    //+ STREETS + LIGHTS ETC
    public:
        Solution* mutate(Solution* solution, int seed, bool ifChangeOrder, bool ifDelete, bool ifChangeLen);
        int streets;
        int lights;
        void createPool(Solution* BaseSolution);
        void setProblem(Problem* problem);
        void getBestFit(Solution* solution);
        //to make it oop we could have setters but cmon
        float MeanCloggStr;
        float MeanCloggLight;
        float SdCloggStr;
        float SdCloggLight;
        std::unordered_set<int> usedStreets;
        //list of problematic cloggy streets - this will be used for mutation - the more clogged a street, or at least if it is above average the more of a chance for mutation= used for CHANGING TIME
        std::map<double, int> ProblematicStreets;
        //list of problematic cloggy lights - this will be used for mutation - the more clogged a street, or at least if it is above average the more of a chance for mutation = used for changing KOLEJNOŚĆ
        std::map<double, int> ProblematicLights;
        static Solver& getInstance(){
            static Solver instance;
            return instance;
        }
        void go();
        Solution* dropThosePeskyCars(Solution* solution, float thereshold);
    // preprocess - is done inside the provlem

    //epochs of 

    //GA        ?? - stochastic mutation search while mutating= probability of mutation proportional to cloggyness/ it being used

    //hillclimb ??

    //feotype is only clogged/problematic streets



};
