#include "includeAll.h"
#include <iostream>

int main(int argc, char *argv[]) {
    std::string path = argv[1];
    Problem* MyProblem;
    MyProblem = Reader::read(path);
    Solution* base = MyProblem->preprocessor();
    MyProblem->toAnswer();
    try {
        std::cout << "create pool" << std::endl;
        Solver::getInstance().setProblem(MyProblem);
        Solver::getInstance().createPool(base);
        std::cout << "go" << std::endl;
        Solution* final = Solver::getInstance().go();
        MyProblem->Lights = final->Lights;
        int score = MyProblem->simulate();
        std::cout << "final score: " << score << std::endl;
    } catch (const std::runtime_error &re) {
        std::cerr << "Runtime error: " << re.what() << std::endl;
    }
    return 0;
}