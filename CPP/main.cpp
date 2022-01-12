#include "includeAll.h"
#include <iostream>
#include <chrono>

int main(){
    Reader MyReader;
    std::string path = "C:/Users/Windows 10/NINA/UNI/COMBINATORIAL OPTIMIZATION/PROJECT FOR LABS/data/d.txt"; 
    Problem* MyProblem;
    MyProblem = MyReader.read(path);
    Solution* base = MyProblem->preprocessor();
    
    try{
    std::cout<<"create pool"<<std::endl;
    Solver::getInstance().setProblem(MyProblem);
    Solver::getInstance().createPool(base);
    std::cout<<"go"<<std::endl;
    Solver::getInstance().go();

    auto start = std::chrono::high_resolution_clock::now();
    MyProblem->Lights = base->Lights;
    MyProblem->reset();
    int score = MyProblem->simulate();
    Solver::getInstance().setProblem(MyProblem);
    auto stop = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
    std::cout<<"Chuj w dupe syniakowi "<<score<<" time: "<< duration.count() <<"(microsec)" <<std::endl;
    //for(Car* car: MyProblem->Cars){
    //    std::cout<<"priority of car:"<< car->priority
    //}

    }catch(const std::runtime_error& re){
        std::cerr << "Runtime error: " << re.what() << std::endl;
    }

    return 0;
}