#pragma once
#include <iostream>
#include <cmath>
#include <vector>
#include <time.h>
#include <chrono>
#include <thread>
#include <utility>
#include <random>
#include <stdio.h>
using namespace std;

#define PI 3.14159265358979323846

class Function{
protected:
    long long int dimensions;
    long long int precision;
    long long int steps = 1000;
    double a;
    double b;
    double max;
    long long int n; //bitsNeeded
    long long int bitStringLength;
public:
    virtual double calculateFunction(const vector<double> &parameters) = 0;
    void initializeRandom(long long int rep, mt19937_64 &generator);
    void generateRandomBitString(vector<bool> &bitString, mt19937_64 &generator);
    double evaluate(const vector<bool> &sol);    
    
    void hillClimbing_firstImprovement(mt19937_64 &generator);
    void hillClimbing_bestImprovement(mt19937_64& generator);
};
