#include "includeAll.h"
#include <iostream>
#include <chrono>

int main(){
    Reader MyReader;
    std::string path = "C:/Users/Windows 10/NINA/UNI/COMBINATORIAL OPTIMIZATION/PROJECT FOR LABS/data/d.txt"; 
    Problem* MyProblem;
    MyProblem = MyReader.read(path);
    Solution* base = MyProblem->preprocessor();
    Solver::getInstance().setProblem(MyProblem);
    Solver::getInstance().createPool(base);
    Solver::getInstance().go();
    try{
    auto start = std::chrono::high_resolution_clock::now();
    MyProblem->reset();
    int score = MyProblem->simulate();
    auto stop = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
    std::cout<<"Chuj w dupe syniakowi "<<score<<" time: "<< duration.count() <<"(microsec)" <<std::endl;
    }catch(const std::runtime_error& re){
        std::cerr << "Runtime error: " << re.what() << std::endl;
    }
    //cpp basic wersja wymulacji (bez zliczania i bez get best fit, bez resetu)
    try{
    MyProblem->reset();
    auto start = std::chrono::high_resolution_clock::now();
    int score = MyProblem->approxsimulate();
    auto stop = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
    std::cout<<"Chuj w dupe syniakowi "<<score<<" time: "<< duration.count() <<"(microsec)" <<std::endl;
    }catch(const std::runtime_error& re){
        std::cerr << "Runtime error: " << re.what() << std::endl;
    }

    return 0;
}