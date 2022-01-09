#include <string>
#include <sstream>
#include <string>
#include <fstream>
#include <iterator>
#include <cassert>
#include <iostream>
#include "problem.h"


class Reader{
    public:

    Problem* read(std::string path){
        std::string line;
        std::ifstream myfile (path);
        Problem* MyProblem;
        if (myfile.is_open())
        {
            std::getline(myfile,line);
            std::istringstream ss(line);
            std::istream_iterator<std::string> begin(ss), end;
            std::vector<std::string> words(begin, end);
            int t,c,l,s,p;
            t = stoi(words[0]); l = stoi(words[1]); s = stoi(words[2]); c = stoi(words[3]); p = stoi(words[4]);
            MyProblem = new Problem(t, l, s, c, p);
            
            int lightId = 0;
            while(lightId < MyProblem->lights){
                MyProblem->Lights.push_back( new Light(lightId, MyProblem->T));
                MyProblem->Lights[lightId]->setUsed();
                lightId +=1;
            }

            
            int strId = 0;
            while(strId < MyProblem->streets){
                std::getline(myfile,line);
                std::istringstream ss(line);
                std::istream_iterator<std::string> begin(ss), end;
                std::vector<std::string> words(begin, end);
                MyProblem->Streets.push_back(new Street(strId, stoi(words[0]), stoi(words[1]), stoi(words[3])));
                MyProblem->NameToId[words[2]] = strId;
                MyProblem->IdToName[strId] = words[2];
                strId += 1;
            }
            int carId = 0;
            while(carId < MyProblem->cars){
                std::getline(myfile,line);
                std::istringstream ss(line);
                std::istream_iterator<std::string> begin(ss), end;
                std::vector<std::string> words(begin, end);
                MyProblem->Cars.push_back(new Car(carId, stoi(words[0])));
                for (auto street = words.begin() + 1; street != words.end(); street ++){

                    MyProblem->Cars[carId]->road.push_back(MyProblem->NameToId[*street]);
                }
                carId += 1;
            }
        }

        return MyProblem;
        }
};


