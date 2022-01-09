#pragma once

class Street{
    public:
    int start, end;
    int id; int length;
    std::vector<int> history;

    int passed = 0;
    int entered = 0;

    Street(int, int, int, int);
    bool is_empty();

    int carLeave();
    int carEnter();

    void reset();


};