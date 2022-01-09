#pragma once
#include "light.h"

 class Solution{
    
  static const int primeArray[581];

  //static set of clogged lights for the hash function needed
  static const int lenArrayPrimes = 580;
  public:

   Solution();
   Solution(std::vector<Light*>& Lights);
   ~Solution();
   int score;
   int approxScore; // wsm niepotrzebne - nie eksperymentowałam zbyt dużo z tym approximation
   std::vector<Light*> Lights;
   long getHash();

 };