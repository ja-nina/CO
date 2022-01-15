#include "includeAll.h"
#include <iostream>

int main(int argc, char *argv[]) {
    auto start = std::chrono::high_resolution_clock::now();
    std::string path = argv[1];
    Problem* MyProblem;
    MyProblem = Reader::read(path);
    Solution* base = MyProblem->preprocessor();
    try {
        Solver::getInstance().setProblem(MyProblem);
        Solver::getInstance().createPool(base);
        Solution* final = Solver::getInstance().go(start);
        MyProblem->Lights = final->Lights;
        MyProblem->toAnswer();
    } catch (const std::runtime_error &re) {
        std::cerr << "Runtime error: " << re.what() << std::endl;
    }
    return 0;
}