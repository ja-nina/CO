#pragma once

#include "light.h"

class Solution {
public:
    static const int primeArray[581];
    static const int lenArrayPrimes = 580;
    explicit Solution(std::vector<Light *> &Lights);
    ~Solution();
    std::vector<Light *> Lights;
    long getHash();
};