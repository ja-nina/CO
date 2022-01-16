#include "includeAll.h"
#include <iostream>

int main(int argc, char *argv[]) {
    auto start = std::chrono::high_resolution_clock::now();
    int x = atoi(argv[1]);
    Problem* MyProblem;
    MyProblem = Reader::read();
    if (x == 1) {
        Solution* base = MyProblem->preprocessor();
        MyProblem->Lights = base->Lights;
        MyProblem->toAnswer();
    }
    else if (x == 2) {
        Solution* base = MyProblem->preprocessor();
        Solver::getInstance().setProblem(MyProblem);
        Solver::getInstance().createPool(base);
        Solution* final = Solver::getInstance().go(start);
        MyProblem->Lights = final->Lights;
        int score = MyProblem->simulate();
        std::cout << score << std::endl;
        MyProblem->toAnswer();
    }
    return 0;
}
