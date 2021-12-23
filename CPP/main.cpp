#include "includeAll.h"
#include <iostream>
#include <chrono>

int main(){
    Reader MyReader;
    std::string path = "C:/Users/Windows 10/NINA/UNI/COMBINATORIAL OPTIMIZATION/PROJECT FOR LABS/data/b.txt"; 
    //std::cout<<"I HATE VSCODE"<<std::endl;
    Problem* MyProblem;
    MyProblem = MyReader.read(path);
    MyProblem->preprocessor();
    try{
    auto start = std::chrono::high_resolution_clock::now();
    int score = MyProblem->simulate();
    auto stop = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
    std::cout<<"Chuj w dupe syniakowi "<<score<<" time: "<< duration.count() <<"(microsec)" <<std::endl;
    }catch(const std::runtime_error& re){
        std::cerr << "Runtime error: " << re.what() << std::endl;
    }
    //w cpp basic wersja wymulacji (bez zliczania i bez get best fit, bez resetu)

    return 0;
}